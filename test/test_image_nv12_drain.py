#!/usr/bin/env python3
"""Four full-sized OIMG frames x N=3 through ROS, including shutdown drain.

Only loopback TCP mock transport and host ROS DDS are exercised; no board.
"""
import binascii
import socket
import struct
import subprocess
import sys
import time

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy
from sensor_msgs.msg import CameraInfo, Image

from test_image_nv12_four import four_listeners
from test_image_stream_node import recv_exact, terminate_process

WIDTH, HEIGHT, N = 1280, 1088, 3
SIZE = WIDTH * HEIGHT * 3 // 2


def frame(camera, sequence):
    payload = bytes((camera, sequence)) * (SIZE // 2)
    header = bytearray(48)
    header[:4] = b'OIMG'
    header[4:7] = bytes((1, 1, camera))
    struct.pack_into('>H', header, 8, 48)
    struct.pack_into('>QQIIIII', header, 12, sequence, 1234,
                     WIDTH, HEIGHT, WIDTH, SIZE, binascii.crc32(payload) & 0xffffffff)
    return header + payload


def run(executable):
    base, listeners = four_listeners()
    peers = []
    process = None
    image_nodes = []
    info_nodes = []
    try:
        rclpy.init()
        reliable = QoSProfile(depth=5, reliability=ReliabilityPolicy.RELIABLE)
        images = [[] for _ in range(4)]
        infos = [[] for _ in range(4)]

        def image_callback(index):
            def callback(image):
                images[index].append((image.header.stamp, image.header.frame_id,
                                      image.width, image.height, image.encoding,
                                      len(image.data), bytes(image.data[:2])))
            return callback

        for index in range(4):
            image_node = Node(f'image_drain_images_{index}_test')
            info_node = Node(f'image_drain_info_{index}_test')
            image_nodes.append(image_node)
            info_nodes.append(info_node)
            image_node.create_subscription(
                Image, f'/orbvi/drain/camera_{index}/image',
                image_callback(index), reliable)
            info_node.create_subscription(
                CameraInfo, f'/orbvi/drain/camera_{index}/camera_info',
                infos[index].append, reliable)
        # Match RELIABLE subscribers before the finite stream starts (raw NV12
        # has no subscriber gate). The existing four-camera test covers the
        # compatible BEST_EFFORT reader, which cannot guarantee N.
        process = subprocess.Popen([
            executable, '--ros-args', '-p', 'runtime_mode:=nv12',
            '-p', 'host:=127.0.0.1', '-p', f'base_port:={base}',
            '-p', 'camera_mask:=15', '-p', 'frames_per_camera:=3',
            '-p', 'idle_timeout_sec:=15.0', '-p', 'drain_min_sec:=2.0',
            '-p', 'drain_timeout_sec:=12.0',
            '-p', 'publish_queue_depth:=16', '-p', 'topic_namespace:=/orbvi/drain',
        ], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        for index, listener in enumerate(listeners):
            peer, _ = listener.accept()
            peers.append(peer)
            assert recv_exact(peer, 8) == b'OCTL\x01\x01' + bytes((index, 0))
        deadline = time.monotonic() + 8
        while time.monotonic() < deadline and any(
                image_nodes[i].count_publishers(f'/orbvi/drain/camera_{i}/image') != 1
                for i in range(4)):
            rclpy.spin_once(info_nodes[0], timeout_sec=0.05)
        assert all(image_nodes[i].count_publishers(f'/orbvi/drain/camera_{i}/image') == 1
                   for i in range(4))
        # Give DDS time to exchange discovery endpoint QoS before frame 1.
        time.sleep(0.5)
        start = time.monotonic()
        for sequence in range(1, N+1):
            for index, peer in enumerate(peers):
                peer.sendall(frame(index, sequence))
        # Spin each CameraInfo topic before each Image topic, reproducing the
        # acceptance script's per-topic spin order. Image readers are matched
        # but their Python callbacks are not allowed to run yet.
        deadline = time.monotonic() + 18
        for index, node in enumerate(info_nodes):
            while time.monotonic() < deadline and len(infos[index]) < N:
                rclpy.spin_once(node, timeout_sec=0.05)
        assert all(len(batch) == N for batch in infos), (
            'CameraInfo did not arrive for all four cameras', [len(x) for x in infos])
        time.sleep(0.4)
        assert process.poll() is None, 'Bridge shut down before ROS image readers drained'
        deadline = time.monotonic() + 12
        for index, node in enumerate(image_nodes):
            while time.monotonic() < deadline and len(images[index]) < N:
                rclpy.spin_once(node, timeout_sec=0.05)
        stdout, stderr = process.communicate(timeout=20)
        elapsed = time.monotonic() - start
        assert process.returncode == 0, (
            process.returncode, [len(batch) for batch in images],
            [len(batch) for batch in infos], stdout, stderr)
        assert elapsed >= 1.8, f'publisher shutdown before configured drain: {elapsed:.2f}s'
        assert 'retaining ROS publishers for at least 2.00s (ACK deadline 12.00s)' in stderr, stderr
        for index in range(4):
            assert len(images[index]) == N, f'camera_{index} reliable Image {len(images[index])}/{N}'
            assert len(infos[index]) == N, f'camera_{index} CameraInfo {len(infos[index])}/{N}'
            for sequence, (image, info) in enumerate(zip(images[index], infos[index]), 1):
                stamp, frame_id, width, height, encoding, length, prefix = image
                assert info.header.stamp == stamp and info.header.frame_id == frame_id
                assert frame_id == f'orbvi_camera_{index}'
                assert (width, height, encoding, length, prefix) == (
                    WIDTH, HEIGHT, 'nv12', SIZE, bytes((index, sequence)))
    finally:
        terminate_process(process)
        for peer in peers:
            peer.close()
        for listener in listeners:
            listener.close()
        for node in image_nodes + info_nodes:
            node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    run(sys.argv[1])
