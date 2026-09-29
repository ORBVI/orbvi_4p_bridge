#!/usr/bin/env python3
"""Transport/control tests for the finite one-shot OIMG JPEG bridge.

These tests deliberately use a marker-shaped mock JPEG payload. They verify
OCTL START gating, one connection per camera, multi-subscriber fan-out, stop on
last-subscriber departure, and no reconnect. They do NOT verify JPEG decode
validity; that requires a board-produced JPEG and real-device acceptance.
"""

import binascii
import select
import socket
import struct
import subprocess
import sys
import time

import rclpy
from rclpy.node import Node
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy, qos_profile_sensor_data
from sensor_msgs.msg import CameraInfo, CompressedImage


def free_tcp_port():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.bind(('127.0.0.1', 0))
        return sock.getsockname()[1]


def python_node_args(executable, base_port, frames, idle_timeout):
    return [
        executable,
        '--ros-args',
        '-p', 'runtime_mode:=jpeg',
        '-p', 'host:=127.0.0.1',
        '-p', f'base_port:={base_port}',
        '-p', 'camera_mask:=1',
        '-p', f'frames_per_camera:={frames}',
        '-p', f'idle_timeout_sec:={idle_timeout}',
        '-p', 'connect_timeout_ms:=2000',
        '-p', 'read_timeout_ms:=2000',
        '-p', 'topic_namespace:=/orbvi/test',
        '-p', 'frame_id_prefix:=test_camera_',
    ]


def recv_exact(connection, size, timeout=5.0):
    deadline = time.monotonic() + timeout
    chunks = bytearray()
    while len(chunks) < size and time.monotonic() < deadline:
        connection.settimeout(max(0.01, deadline - time.monotonic()))
        chunk = connection.recv(size - len(chunks))
        if not chunk:
            raise AssertionError('board connection closed before START was read')
        chunks.extend(chunk)
    if len(chunks) != size:
        raise AssertionError(f'timed out reading {size} START bytes')
    return bytes(chunks)


def make_transport_mock_jpeg(sequence=1, width=16, height=16, camera=0):
    # Marker-shaped fixture only. The SDK reference transport validates SOI/EOI;
    # decodability is intentionally outside this host transport test.
    payload = b'\xff\xd8\xff\xe0' + b'bridge-test' + b'\xff\xd9'
    header = bytearray(48)
    header[0:4] = b'OIMG'
    header[4] = 1
    header[5] = 2
    header[6] = camera
    header[7] = 0
    struct.pack_into('>H', header, 8, 48)
    struct.pack_into('>H', header, 10, 0)
    struct.pack_into('>Q', header, 12, sequence)
    struct.pack_into('>Q', header, 20, 123456789)
    struct.pack_into('>I', header, 28, width)
    struct.pack_into('>I', header, 32, height)
    struct.pack_into('>I', header, 36, 0)
    struct.pack_into('>I', header, 40, len(payload))
    struct.pack_into('>I', header, 44, binascii.crc32(payload) & 0xffffffff)
    return bytes(header) + payload


def terminate_process(process):
    if process is None or process.poll() is not None:
        return
    process.terminate()
    try:
        process.wait(timeout=3.0)
    except subprocess.TimeoutExpired:
        process.kill()
        process.wait(timeout=3.0)


def test_jpeg_without_subscriber_has_no_start(executable):
    port = free_tcp_port()
    listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listener.bind(('127.0.0.1', port))
    listener.listen(2)
    listener.settimeout(0.4)

    process = subprocess.Popen(
        python_node_args(executable, port, 1, 0.5),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        return_code = process.wait(timeout=8.0)
        stdout, stderr = process.communicate()
        if return_code == 0 or "subscriber idle timeout" not in stderr:
            raise AssertionError(
                f'idle JPEG node should fail without N; exited {return_code}\n'
                f'stdout={stdout}\nstderr={stderr}'
            )
        try:
            connection, _ = listener.accept()
        except socket.timeout:
            connection = None
        if connection is not None:
            connection.close()
            raise AssertionError('JPEG bridge connected without a ROS subscriber')
    finally:
        terminate_process(process)
        listener.close()


def test_two_subscribers_share_one_one_shot_session(executable):
    rclpy.init()
    ros_node = Node('bridge_image_test')
    messages = [[], []]
    camera_info_messages = []
    qos = QoSProfile(
        depth=5,
        reliability=ReliabilityPolicy.BEST_EFFORT,
        durability=DurabilityPolicy.VOLATILE,
    )
    subscriptions = [
        ros_node.create_subscription(
            CompressedImage,
            '/orbvi/test/camera_0/image/compressed',
            messages[0].append,
            qos,
        ),
        ros_node.create_subscription(
            CompressedImage,
            '/orbvi/test/camera_0/image/compressed',
            messages[1].append,
            qos,
        ),
    ]
    camera_info_subscription = ros_node.create_subscription(
        CameraInfo,
        '/orbvi/test/camera_0/camera_info',
        camera_info_messages.append,
        qos,
    )

    port = free_tcp_port()
    listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listener.bind(('127.0.0.1', port))
    listener.listen(3)
    listener.settimeout(0.05)

    process = subprocess.Popen(
        python_node_args(executable, port, 3, 8.0),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    connection = None
    try:
        deadline = time.monotonic() + 10.0
        while connection is None and time.monotonic() < deadline:
            if process.poll() is not None:
                stdout, stderr = process.communicate()
                raise AssertionError(
                    f'image node exited early {process.returncode}\n'
                    f'stdout={stdout}\nstderr={stderr}'
                )
            rclpy.spin_once(ros_node, timeout_sec=0.05)
            try:
                connection, _ = listener.accept()
            except socket.timeout:
                pass
        if connection is None:
            raise AssertionError('no board connection after ROS subscriber appeared')

        start = recv_exact(connection, 8)
        if start != b'OCTL\x01\x01\x00\x00':
            raise AssertionError(f'unexpected OCTL START bytes: {start!r}')

        # A second subscriber must not create a second SDK connection.
        listener.settimeout(0.5)
        try:
            duplicate, _ = listener.accept()
        except socket.timeout:
            duplicate = None
        if duplicate is not None:
            duplicate.close()
            raise AssertionError('second subscriber created a duplicate SDK connection')

        connection.sendall(make_transport_mock_jpeg())

        deadline = time.monotonic() + 10.0
        while time.monotonic() < deadline and (
            not messages[0] or not messages[1] or not camera_info_messages
        ):
            rclpy.spin_once(ros_node, timeout_sec=0.1)
        if not messages[0] or not messages[1]:
            raise AssertionError('both subscribers did not receive the first JPEG')
        if not camera_info_messages:
            raise AssertionError('CameraInfo was not published with the first JPEG')

        first = messages[0][0]
        if first.format != 'jpeg' or bytes(first.data[:2]) != b'\xff\xd8':
            raise AssertionError(f'invalid CompressedImage: format={first.format!r}')
        if first.header.frame_id != 'test_camera_0':
            raise AssertionError(f'unexpected frame_id: {first.header.frame_id!r}')
        info = camera_info_messages[0]
        if info.header != first.header:
            raise AssertionError('CameraInfo header differs from CompressedImage')
        if (info.width, info.height) != (16, 16):
            raise AssertionError('CameraInfo dimensions do not match the frame')
        if any(info.k) or any(info.p) or list(info.d):
            raise AssertionError('uncalibrated CameraInfo fabricated calibration')

        # A subscriber arriving after START must share this same connection.
        late_messages = []
        late_subscription = ros_node.create_subscription(
            CompressedImage, '/orbvi/test/camera_0/image/compressed',
            late_messages.append, qos,
        )
        subscriptions.append(late_subscription)
        deadline = time.monotonic() + 5.0
        while time.monotonic() < deadline and ros_node.count_subscribers('/orbvi/test/camera_0/image/compressed') < 3:
            rclpy.spin_once(ros_node, timeout_sec=0.05)
        if ros_node.count_subscribers('/orbvi/test/camera_0/image/compressed') < 3:
            raise AssertionError('late subscriber did not join the active session')
        connection.sendall(make_transport_mock_jpeg(sequence=2))
        deadline = time.monotonic() + 5.0
        while time.monotonic() < deadline and not late_messages:
            rclpy.spin_once(ros_node, timeout_sec=0.05)
        if not late_messages:
            raise AssertionError('late subscriber did not receive the next frame')

        # Dropping one of two subscribers must not close or duplicate the one
        # already-running one-shot session.
        ros_node.destroy_subscription(subscriptions[1])
        ros_node.destroy_subscription(late_subscription)
        deadline = time.monotonic() + 1.0
        while time.monotonic() < deadline:
            rclpy.spin_once(ros_node, timeout_sec=0.05)
        readable, _, _ = select.select([connection], [], [], 0.4)
        if readable:
            peek = connection.recv(1, socket.MSG_PEEK)
            if peek == b'':
                raise AssertionError('session stopped while a subscriber remained')
        listener.settimeout(0.4)
        try:
            duplicate, _ = listener.accept()
        except socket.timeout:
            duplicate = None
        if duplicate is not None:
            duplicate.close()
            raise AssertionError('subscriber departure caused a reconnect')

        # Dropping the last subscriber must stop the one-shot session quickly.
        ros_node.destroy_subscription(subscriptions[0])
        deadline = time.monotonic() + 10.0
        while time.monotonic() < deadline and process.poll() is None:
            rclpy.spin_once(ros_node, timeout_sec=0.1)
        if process.poll() is None:
            raise AssertionError('image node did not exit after last subscriber left')
        stdout, stderr = process.communicate()
        if process.returncode == 0 or "subscriber disappeared" not in stderr:
            raise AssertionError(
                f'incomplete image node should fail; exited {process.returncode}\n'
                f'stdout={stdout}\nstderr={stderr}'
            )

        connection.settimeout(2.0)
        try:
            closed = connection.recv(1)
        except socket.timeout:
            raise AssertionError('board connection stayed open after terminal cancellation')
        if closed != b'':
            raise AssertionError('unexpected trailing data after terminal cancellation')

        # No second connection/START is allowed after terminal cancellation.
        listener.settimeout(1.0)
        try:
            reconnect, _ = listener.accept()
        except socket.timeout:
            reconnect = None
        if reconnect is not None:
            reconnect.close()
            raise AssertionError('completed/cancelled camera received a second connection')
    finally:
        terminate_process(process)
        if connection is not None:
            connection.close()
        listener.close()
        for subscription in subscriptions:
            try:
                ros_node.destroy_subscription(subscription)
            except Exception:
                pass
        ros_node.destroy_subscription(camera_info_subscription)
        ros_node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


def test_active_idle_timeout_does_not_reconnect(executable):
    """Board stalls after START: nonzero exit and no second START/connection."""
    port = free_tcp_port()
    listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listener.bind(('127.0.0.1', port))
    listener.listen(2)
    listener.settimeout(4.0)
    process = subprocess.Popen(
        python_node_args(executable, port, 2, 0.8),
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
    )
    connection = None
    try:
        # A listener that never sends frames must not cause an infinite wait.
        rclpy.init()
        ros_node = Node('image_stall_test')
        qos = QoSProfile(depth=5, reliability=ReliabilityPolicy.BEST_EFFORT)
        sub = ros_node.create_subscription(
            CompressedImage, '/orbvi/test/camera_0/image/compressed',
            lambda _message: None, qos)
        try:
            deadline = time.monotonic() + 4.0
            while time.monotonic() < deadline and connection is None:
                rclpy.spin_once(ros_node, timeout_sec=0.05)
                listener.settimeout(0.05)
                try:
                    connection, _ = listener.accept()
                except socket.timeout:
                    pass
            if connection is None:
                raise AssertionError('subscriber did not trigger START')
            if recv_exact(connection, 8) != b'OCTL\x01\x01\x00\x00':
                raise AssertionError('missing START on stalled session')
            code = process.wait(timeout=6.0)
            stdout, stderr = process.communicate()
            if code == 0 or 'idle timeout before requested N' not in stderr:
                raise AssertionError(f'stall did not fail: {code} {stdout} {stderr}')
            listener.settimeout(0.4)
            try:
                retry, _ = listener.accept()
            except socket.timeout:
                retry = None
            if retry:
                retry.close()
                raise AssertionError('session reconnected after finite N failure')
        finally:
            ros_node.destroy_subscription(sub)
            ros_node.destroy_node()
            rclpy.shutdown()
    finally:
        terminate_process(process)
        if connection:
            connection.close()
        listener.close()


def test_bad_crc_does_not_count_as_finite_frame(executable):
    """A corrupt wire frame must not satisfy finite N or emit CameraInfo."""
    port = free_tcp_port()
    listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listener.bind(('127.0.0.1', port))
    listener.listen(2)
    process = None
    connection = None
    node = None
    try:
        rclpy.init()
        node = Node('image_corrupt_test')
        messages, infos = [], []
        node.create_subscription(
            CompressedImage, '/orbvi/test/camera_0/image/compressed',
            messages.append, qos_profile_sensor_data)
        node.create_subscription(
            CameraInfo, '/orbvi/test/camera_0/camera_info',
            infos.append, qos_profile_sensor_data)
        process = subprocess.Popen(
            python_node_args(executable, port, 1, 1.0),
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        listener.settimeout(5)
        connection, _ = listener.accept()
        assert recv_exact(connection, 8) == b'OCTL\x01\x01\x00\x00'
        topic = '/orbvi/test/camera_0/image/compressed'
        deadline = time.monotonic() + 5
        while time.monotonic() < deadline and node.count_publishers(topic) == 0:
            rclpy.spin_once(node, timeout_sec=0.05)
        assert node.count_publishers(topic) == 1
        corrupted = bytearray(make_transport_mock_jpeg())
        corrupted[-1] ^= 1
        connection.sendall(corrupted)
        deadline = time.monotonic() + 6
        while time.monotonic() < deadline and process.poll() is None:
            rclpy.spin_once(node, timeout_sec=0.05)
        assert process.poll() is not None, 'bad CRC session did not terminate'
        stdout, stderr = process.communicate()
        assert process.returncode != 0 and 'idle timeout before requested N' in stderr, (stdout, stderr)
        assert not messages and not infos, 'corrupt frame reached ROS'
        listener.settimeout(0.2)
        try:
            extra, _ = listener.accept()
        except socket.timeout:
            extra = None
        if extra is not None:
            extra.close()
            raise AssertionError('corrupt finite session reconnected')
    finally:
        terminate_process(process)
        if connection:
            connection.close()
        listener.close()
        if node:
            node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


def main():
    if len(sys.argv) != 2:
        print('usage: test_image_stream_node.py /path/to/orbvi_image_stream_node', file=sys.stderr)
        return 2
    executable = sys.argv[1]
    test_jpeg_without_subscriber_has_no_start(executable)
    test_two_subscribers_share_one_one_shot_session(executable)
    test_active_idle_timeout_does_not_reconnect(executable)
    test_bad_crc_does_not_count_as_finite_frame(executable)
    print('PASS: mock OIMG JPEG START is subscriber-gated and per-camera one-shot; decode not tested')
    return 0


if __name__ == '__main__':
    sys.exit(main())
