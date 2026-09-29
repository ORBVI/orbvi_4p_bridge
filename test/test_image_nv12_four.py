#!/usr/bin/env python3
"""Four-port OIMG NV12 -> ROS Image/CameraInfo reference transport test."""
import binascii
import socket
import struct
import subprocess
import sys
import time

import rclpy
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy
from sensor_msgs.msg import CameraInfo, Image


def four_listeners():
    # Keep all four sockets bound across the entire test; never race a free-port query.
    for _ in range(100):
        listeners = []
        base = 65536
        succeeded = False
        try:
            first = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            first.bind(('127.0.0.1', 0))
            base = first.getsockname()[1]
            listeners.append(first)
            if base > 65532:
                continue
            for index in range(1, 4):
                sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                listeners.append(sock)
                sock.bind(('127.0.0.1', base + index))
            for sock in listeners:
                sock.listen(1)
                sock.settimeout(8)
            succeeded = True
            return base, listeners
        except OSError:
            pass
        finally:
            if not succeeded:
                for sock in listeners:
                    sock.close()
    raise AssertionError('unable to bind four adjacent loopback ports')


def frame(camera, sequence):
    payload = bytes((camera * 24 + pixel) & 255 for pixel in range(24))
    header = bytearray(48)
    header[:4] = b'OIMG'
    header[4:7] = bytes((1, 1, camera))
    struct.pack_into('>H', header, 8, 48)
    struct.pack_into('>QQIIIII', header, 12, sequence, 1234, 4, 4, 4,
                     len(payload), binascii.crc32(payload) & 0xffffffff)
    return bytes(header) + payload, payload


def run(executable):
    base, listeners = four_listeners()
    rclpy.init()
    node = Node('bridge_nv12_four_test')
    qos = QoSProfile(depth=8, reliability=ReliabilityPolicy.BEST_EFFORT,
                     durability=DurabilityPolicy.VOLATILE)
    images = [[] for _ in range(4)]
    infos = [[] for _ in range(4)]
    subscriptions = []
    for index in range(4):
        subscriptions.append(node.create_subscription(
            Image, f'/orbvi/testnv/camera_{index}/image', images[index].append, qos))
        subscriptions.append(node.create_subscription(
            CameraInfo, f'/orbvi/testnv/camera_{index}/camera_info', infos[index].append, qos))
    process = subprocess.Popen([
        executable, '--ros-args', '-p', 'runtime_mode:=nv12',
        '-p', 'host:=127.0.0.1', '-p', f'base_port:={base}',
        '-p', 'camera_mask:=15', '-p', 'frames_per_camera:=2',
        '-p', 'idle_timeout_sec:=12.0', '-p', 'connect_timeout_ms:=6000',
        '-p', 'read_timeout_ms:=6000', '-p', 'topic_namespace:=/orbvi/testnv',
    ], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    peers = []
    try:
        for index, listener in enumerate(listeners):
            peer, _ = listener.accept()
            peers.append(peer)
            peer.settimeout(5)
            control = bytearray()
            while len(control) < 8:
                chunk = peer.recv(8 - len(control))
                assert chunk, "connection closed before START"
                control.extend(chunk)
            assert bytes(control) == b'OCTL\x01\x01' + bytes((index, 0)), control
        # Allow DDS discovery before sending the finite one-shot frame.
        deadline = time.monotonic() + 5
        while time.monotonic() < deadline and any(
                node.count_publishers(f'/orbvi/testnv/camera_{index}/image') == 0
                for index in range(4)):
            rclpy.spin_once(node, timeout_sec=0.05)
        time.sleep(0.4)
        expected = []
        for index, peer in enumerate(peers):
            wire, payload = frame(index, 1)
            expected.append(payload)
            peer.sendall(wire)
        deadline = time.monotonic() + 8
        while time.monotonic() < deadline and any(len(x) < 1 for x in images + infos):
            rclpy.spin_once(node, timeout_sec=0.05)
        assert all(len(x) == 1 for x in images + infos), 'first frames not received'
        # A finished camera must not close the whole node while peers still
        # need their second frame. Exercise completion in a different order.
        for index in (3, 1, 0, 2):
            wire, _ = frame(index, 2)
            peers[index].sendall(wire)
            deadline = time.monotonic() + 8
            while time.monotonic() < deadline and len(images[index]) < 2:
                rclpy.spin_once(node, timeout_sec=0.05)
            assert len(images[index]) == 2, f'camera {index} did not finish independently'
            if index != 2:
                assert process.poll() is None, 'node exited before all cameras finished'
        deadline = time.monotonic() + 3
        while time.monotonic() < deadline and any(len(x) < 2 for x in infos):
            rclpy.spin_once(node, timeout_sec=0.05)
        for index in range(4):
            assert len(images[index]) == 2, f'camera {index} Image count {len(images[index])}'
            assert len(infos[index]) == 2, f'camera {index} CameraInfo count {len(infos[index])}'
            for image, info in zip(images[index], infos[index]):
                assert (image.width, image.height, image.step, image.encoding) == (4, 4, 4, 'nv12')
                assert bytes(image.data) == expected[index]
                assert info.header == image.header
                assert image.header.frame_id == f'orbvi_camera_{index}'
                assert image.header.stamp.sec > 1600000000
                assert info.width == 4 and info.height == 4
                assert not any(info.k) and not any(info.p) and not info.d
        output, errors = process.communicate(timeout=5)
        assert process.returncode == 0, (process.returncode, output, errors)
    finally:
        if process.poll() is None:
            process.terminate()
            try:
                process.wait(timeout=3)
            except subprocess.TimeoutExpired:
                process.kill()
                process.wait(timeout=3)
        for peer in peers:
            peer.close()
        for listener in listeners:
            listener.close()
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    run(sys.argv[1])
