#!/usr/bin/env python3
"""OCPU/1 UDP -> SDK on_temperature -> ROS Temperature end-to-end test."""

import binascii
import socket
import struct
import subprocess
import sys
import time

import rclpy
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import Temperature

GOLDEN = bytes.fromhex(
    '4f 43 50 55 01 01 00 00 '
    '01 02 03 04 05 06 07 08 '
    '11 12 13 14 15 16 17 18 '
    '41 cc 00 00 4a e5 12 f5'
)


def free_udp_port():
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.bind(('127.0.0.1', 0))
        return sock.getsockname()[1]


def make_ocpu_packet(celsius_bits, sequence=1, monotonic_ns=0x1112131415161718):
    body = bytearray(28)
    body[0:4] = b'OCPU'
    body[4] = 1
    body[5] = 1
    body[6:8] = b'\x00\x00'
    struct.pack_into('>Q', body, 8, sequence)
    struct.pack_into('>Q', body, 16, monotonic_ns)
    struct.pack_into('>I', body, 24, celsius_bits)
    crc = binascii.crc32(body) & 0xffffffff
    return bytes(body) + struct.pack('>I', crc)


def terminate_process(process):
    if process is None or process.poll() is not None:
        return
    process.terminate()
    try:
        process.wait(timeout=3.0)
    except subprocess.TimeoutExpired:
        process.kill()
        process.wait(timeout=3.0)


def test_incomplete_finite_sample_timeout(executable):
    port = free_udp_port()
    process = subprocess.Popen([
        executable, '--ros-args', '-p', 'bind_address:=127.0.0.1',
        '-p', f'port:={port}', '-p', 'samples:=2',
        '-p', 'idle_timeout_sec:=0.7',
    ], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    try:
        # No datagrams are sent; packet loss (including all packets) must fail.
        code = process.wait(timeout=5.0)
        stdout, stderr = process.communicate()
        if code == 0 or 'OCPU idle timeout: received 0/2' not in stderr:
            raise AssertionError(f'missing bounded failure: {code} {stdout} {stderr}')
    finally:
        terminate_process(process)


def main():
    if len(sys.argv) != 2:
        print('usage: test_temperature_node.py /path/to/orbvi_temperature_node', file=sys.stderr)
        return 2
    executable = sys.argv[1]
    test_incomplete_finite_sample_timeout(executable)
    port = free_udp_port()
    topic = '/orbvi/test/temperature'

    rclpy.init()
    ros_node = Node('temperature_bridge_test')
    messages = []
    subscription = ros_node.create_subscription(
        Temperature, topic, messages.append, qos_profile_sensor_data)
    process = subprocess.Popen(
        [
            executable,
            '--ros-args',
            '-p', 'bind_address:=127.0.0.1',
            '-p', f'port:={port}',
            '-p', 'samples:=1',
            '-p', f'topic:={topic}',
            '-p', 'source:=cpu',
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        deadline = time.monotonic() + 10.0
        while time.monotonic() < deadline and ros_node.count_publishers(topic) == 0:
            if process.poll() is not None:
                stdout, stderr = process.communicate()
                raise AssertionError(
                    f'temperature node exited early {process.returncode}\n'
                    f'stdout={stdout}\nstderr={stderr}'
                )
            rclpy.spin_once(ros_node, timeout_sec=0.1)
        if ros_node.count_publishers(topic) == 0:
            raise AssertionError('temperature publisher was not discovered')
        topic_types = dict(ros_node.get_topic_names_and_types())
        if topic_types.get(topic) != ['sensor_msgs/msg/Temperature']:
            raise AssertionError(f'unexpected topic type: {topic_types.get(topic)!r}')

        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sender:
            bad_crc = bytearray(GOLDEN)
            bad_crc[-1] ^= 0x01
            sender.sendto(bad_crc, ('127.0.0.1', port))
            nan_packet = make_ocpu_packet(0x7fc00000, sequence=1)
            sender.sendto(nan_packet, ('127.0.0.1', port))
            deadline = time.monotonic() + 0.6
            while time.monotonic() < deadline:
                rclpy.spin_once(ros_node, timeout_sec=0.05)
            if messages:
                raise AssertionError('bad CRC or NaN OCPU packet was published')

            deadline = time.monotonic() + 8.0
            while time.monotonic() < deadline and not messages:
                sender.sendto(GOLDEN, ('127.0.0.1', port))
                rclpy.spin_once(ros_node, timeout_sec=0.1)
            if not messages:
                raise AssertionError('no ROS Temperature received from golden OCPU/1')

        return_code = process.wait(timeout=8.0)
        if return_code != 0:
            stdout, stderr = process.communicate()
            raise AssertionError(
                f'temperature node exited {return_code}\nstdout={stdout}\nstderr={stderr}'
            )
        if len(messages) != 1:
            raise AssertionError(f'finite samples should stop after one packet; got {len(messages)}')

        message = messages[0]
        if abs(message.temperature - 25.5) > 1e-6:
            raise AssertionError(f'unexpected temperature: {message.temperature}')
        if message.variance != 0.0:
            raise AssertionError(f'unexpected variance: {message.variance}')
        if message.header.frame_id != 'cpu':
            raise AssertionError(f'unexpected source identifier: {message.header.frame_id!r}')
        if not 1600000000 < message.header.stamp.sec < 2147483647:
            raise AssertionError(f'ROS stamp is not host wall time: {message.header.stamp}')
        print('PASS: OCPU/1 CPU temperature -> sensor_msgs/msg/Temperature')
        return 0
    finally:
        terminate_process(process)
        ros_node.destroy_subscription(subscription)
        ros_node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    sys.exit(main())
