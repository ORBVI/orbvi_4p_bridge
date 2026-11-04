"""Verify board timestamps and metadata reach the ROS image and IMU topics."""

import socket
import time
import unittest

import launch
import launch_ros.actions
import launch_testing.actions
import rclpy
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import Image, Imu
from orbvi_4p_bridge.msg import FrameMetadata, ImuMetadata

PORT = 15674
GOLDEN = bytes.fromhex(
    '4f 49 4d 55 01 01 00 48 01 02 03 04 05 06 07 08 '
    '11 12 13 14 15 16 17 18 21 22 23 24 25 26 27 28 '
    '31 32 33 34 35 36 37 38 00 00 00 00 3f 80 00 00 '
    'c0 20 00 00 40 50 00 00 c0 80 00 00 40 b0 00 00 '
    'c0 d8 00 00 f3 60 80 31'
)


def generate_test_description():
    camera = launch_ros.actions.Node(
        package='orbvi_4p_bridge', executable='orbvi_bridge_node',
        name='capture_camera_test', output='screen', parameters=[{
            'runtime_mode': 'fake', 'camera_count': 4, 'frames_per_camera': 300,
            'emit_interval_us': 100000, 'frame_width': 64, 'frame_height': 48,
            'timestamp_source': 'board', 'publish_capture_metadata': True,
            'topic_namespace': '/orbvi/capture_test',
        }])
    imu = launch_ros.actions.Node(
        package='orbvi_4p_bridge', executable='orbvi_bridge_node',
        name='capture_imu_test', output='screen', parameters=[{
            'runtime_mode': 'udp_imu', 'imu_enabled': True,
            'imu_bind_address': '127.0.0.1', 'imu_port': PORT,
            'timestamp_source': 'board', 'publish_capture_metadata': True,
            'topic_namespace': '/orbvi/capture_imu_test',
        }])
    return launch.LaunchDescription([
        camera, imu, launch_testing.actions.ReadyToTest(),
    ])


class TestBoardCapture(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        rclpy.init()

    @classmethod
    def tearDownClass(cls):
        rclpy.shutdown()

    def test_capture_metadata(self):
        node = rclpy.create_node('board_capture_test')
        self.addCleanup(node.destroy_node)
        images, frames, imus, imu_metadata = [], [], [], []
        specs = [
            (Image, '/orbvi/capture_test/camera_0/image', images),
            (FrameMetadata, '/orbvi/capture_test/camera_0/metadata', frames),
            (Imu, '/orbvi/capture_imu_test/imu', imus),
            (ImuMetadata, '/orbvi/capture_imu_test/imu/metadata', imu_metadata),
        ]
        subscriptions = [node.create_subscription(kind, topic, values.append,
                                                   qos_profile_sensor_data)
                         for kind, topic, values in specs]
        for subscription in subscriptions:
            self.addCleanup(node.destroy_subscription, subscription)
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sender:
            deadline = time.monotonic() + 15.0
            while time.monotonic() < deadline:
                sender.sendto(GOLDEN, ('127.0.0.1', PORT))
                rclpy.spin_once(node, timeout_sec=0.05)
                image_stamps = {(msg.header.stamp.sec, msg.header.stamp.nanosec)
                                for msg in images}
                if imus and imu_metadata and any(
                        (msg.header.stamp.sec, msg.header.stamp.nanosec) in image_stamps
                        for msg in frames):
                    break

        self.assertTrue(images and frames and imus and imu_metadata)
        frame = next(msg for msg in frames
                     if (msg.header.stamp.sec, msg.header.stamp.nanosec) in image_stamps)
        frame_stamp = frame.header.stamp.sec * 1000000000 + frame.header.stamp.nanosec
        self.assertEqual(frame_stamp, frame.sensor_timestamp_ns)
        self.assertEqual(frame.sensor_timestamp_ns, frame.sequence * 1000000)
        accel_ns = int.from_bytes(GOLDEN[16:24], 'big')
        gyro_ns = int.from_bytes(GOLDEN[24:32], 'big')
        imu_stamp = imus[0].header.stamp.sec * 1000000000 + imus[0].header.stamp.nanosec
        self.assertEqual(imu_stamp, min(accel_ns, gyro_ns))
        self.assertEqual(imu_metadata[0].header.stamp, imus[0].header.stamp)
        self.assertEqual(imu_metadata[0].accel_timestamp_ns, accel_ns)
        self.assertEqual(imu_metadata[0].gyro_timestamp_ns, gyro_ns)
