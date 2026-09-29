#!/usr/bin/env python3
"""Four independent JPEG sessions: only compressed subscribers trigger START.

Marker-shaped payloads test OIMG/ROS transport, not JPEG decoding or hardware.
"""
import socket
import subprocess
import sys
import tempfile
from pathlib import Path
import time

import rclpy
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import CameraInfo, CompressedImage

from test_image_nv12_four import four_listeners
from test_image_stream_node import make_transport_mock_jpeg, recv_exact, terminate_process


def run(executable):
    base, listeners = four_listeners()
    node = None
    process = None
    peers = []
    calibration_dir = tempfile.TemporaryDirectory(prefix='orbvi-test-calibration-')
    calibration_path = Path(calibration_dir.name) / 'camera_2.yaml'
    # Synthetic calibration checks YAML plumbing only, not optical accuracy.
    calibration_path.write_text('''image_width: 20
image_height: 16
distortion_model: plumb_bob
distortion_coefficients: {rows: 1, cols: 5, data: [0, 0, 0, 0, 0]}
camera_matrix: {rows: 3, cols: 3, data: [100, 0, 10, 0, 110, 8, 0, 0, 1]}
rectification_matrix: {rows: 3, cols: 3, data: [1, 0, 0, 0, 1, 0, 0, 0, 1]}
projection_matrix: {rows: 3, cols: 4, data: [100, 0, 10, 0, 0, 110, 8, 0, 0, 0, 1, 0]}
''')
    try:
        rclpy.init()
        node = Node('bridge_jpeg_four_test')
        received = [[] for _ in range(4)]
        infos = [[] for _ in range(4)]
        _info_subs = [node.create_subscription(
            CameraInfo, f'/orbvi/testjpeg/camera_{i}/camera_info',
            infos[i].append, qos_profile_sensor_data) for i in range(4)]
        process = subprocess.Popen([
            executable, '--ros-args', '-p', 'runtime_mode:=jpeg',
            '-p', 'host:=127.0.0.1', '-p', f'base_port:={base}',
            '-p', 'camera_mask:=15', '-p', 'frames_per_camera:=1',
            '-p', 'idle_timeout_sec:=12.0', '-p', 'topic_namespace:=/orbvi/testjpeg',
            '-p', f'calibration_2:={calibration_path}',
        ], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        # Info-only subscribers on all four cameras must not open a TCP socket.
        for index in range(4):
            topic = f'/orbvi/testjpeg/camera_{index}/camera_info'
            deadline = time.monotonic() + 5
            while time.monotonic() < deadline and node.count_publishers(topic) == 0:
                rclpy.spin_once(node, timeout_sec=0.05)
            assert node.count_publishers(topic) == 1, f'camera {index} not discovered'
        for listener in listeners:
            listener.settimeout(0.2)
            try:
                peer, _ = listener.accept()
            except socket.timeout:
                continue
            peer.close()
            raise AssertionError('CameraInfo-only subscription triggered JPEG START')

        image_subs = []
        for index in (2, 0, 3, 1):
            topic = f'/orbvi/testjpeg/camera_{index}/image/compressed'
            image_subs.append(node.create_subscription(
                CompressedImage, topic, received[index].append, qos_profile_sensor_data))
            deadline = time.monotonic() + 5
            while time.monotonic() < deadline and node.count_publishers(topic) == 0:
                rclpy.spin_once(node, timeout_sec=0.05)
            assert node.count_publishers(topic) == 1
            listener = listeners[index]
            listener.settimeout(5)
            peer, _ = listener.accept()
            peers.append(peer)
            assert recv_exact(peer, 8) == b'OCTL\x01\x01' + bytes((index, 0))
            peer.sendall(make_transport_mock_jpeg(camera=index, width=16+index*2))
            deadline = time.monotonic() + 5
            while time.monotonic() < deadline and (not received[index] or not infos[index]):
                rclpy.spin_once(node, timeout_sec=0.05)
            assert len(received[index]) == len(infos[index]) == 1, f'camera {index} missing output'
            image, info = received[index][0], infos[index][0]
            assert image.format == 'jpeg'
            assert bytes(image.data).startswith(b'\xff\xd8')
            assert image.header == info.header
            assert image.header.frame_id == f'orbvi_camera_{index}'
            assert (info.width, info.height) == (16+index*2, 16)
            if index == 2:
                assert info.k[0] == 100 and info.k[4] == 110
                assert info.p[0] == 100 and info.p[5] == 110
                assert len(info.d) == 5 and info.distortion_model == 'plumb_bob'
            else:
                assert not any(info.k) and not any(info.p) and not info.d
            if index != 1:
                assert process.poll() is None, 'one camera completion terminated other cameras'
        stdout, stderr = process.communicate(timeout=5)
        assert process.returncode == 0, (stdout, stderr)
        for index, listener in enumerate(listeners):
            listener.settimeout(0.1)
            try:
                extra, _ = listener.accept()
            except socket.timeout:
                continue
            extra.close()
            raise AssertionError(f'camera {index} opened more than one session')
    finally:
        terminate_process(process)
        for peer in peers:
            peer.close()
        for listener in listeners:
            listener.close()
        if node is not None:
            node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()
        calibration_dir.cleanup()


if __name__ == '__main__':
    run(sys.argv[1])
