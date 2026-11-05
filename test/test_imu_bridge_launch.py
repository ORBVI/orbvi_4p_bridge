"""OIMU/1 UDP -> SDK on_imu -> ROS Imu end-to-end test on localhost."""

import socket
import time
import unittest

import launch
import launch_ros.actions
import launch_testing.actions
import rclpy
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy
from sensor_msgs.msg import Imu

PORT = 15673
GOLDEN = bytes.fromhex(
    '4f 49 4d 55 01 01 00 48 01 02 03 04 05 06 07 08 '
    '11 12 13 14 15 16 17 18 21 22 23 24 25 26 27 28 '
    '31 32 33 34 35 36 37 38 00 00 00 00 3f 80 00 00 '
    'c0 20 00 00 40 50 00 00 c0 80 00 00 40 b0 00 00 '
    'c0 d8 00 00 f3 60 80 31'
)


def generate_test_description():
    bridge = launch_ros.actions.Node(
        package='orbvi_4p_bridge', executable='orbvi_bridge_node',
        name='orbvi_4p_bridge', output='screen', parameters=[{
            'runtime_mode': 'udp_imu', 'imu_enabled': True,
            'imu_bind_address': '127.0.0.1', 'imu_port': PORT,
            'imu_frame_id': 'test_imu', 'topic_namespace': '/orbvi/test',
        }],
    )
    return launch.LaunchDescription([
        bridge, launch_testing.actions.ReadyToTest(),
    ]), {'bridge_node': bridge}


class TestImuBridge(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        rclpy.init()

    @classmethod
    def tearDownClass(cls):
        rclpy.shutdown()

    def test_golden_and_reject_bad_crc(self):
        node = rclpy.create_node('imu_bridge_test')
        self.addCleanup(node.destroy_node)
        messages = []
        qos = QoSProfile(depth=5, reliability=ReliabilityPolicy.BEST_EFFORT,
                         durability=DurabilityPolicy.VOLATILE)
        subscription = node.create_subscription(
            Imu, '/orbvi/test/imu', messages.append, qos)
        self.addCleanup(node.destroy_subscription, subscription)
        deadline = time.monotonic() + 10.0
        while time.monotonic() < deadline and node.count_publishers('/orbvi/test/imu') == 0:
            rclpy.spin_once(node, timeout_sec=0.1)
        self.assertGreater(node.count_publishers('/orbvi/test/imu'), 0)
        self.assertEqual(dict(node.get_topic_names_and_types())['/orbvi/test/imu'],
                         ['sensor_msgs/msg/Imu'])
        self.assertEqual(node.count_publishers('/orbvi/test/imu/metadata'), 0)

        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sender:
            bad = bytearray(GOLDEN)
            bad[44] ^= 1  # CRC no longer matches.
            for _ in range(4):
                sender.sendto(bad, ('127.0.0.1', PORT))
                rclpy.spin_once(node, timeout_sec=0.05)
            self.assertEqual(messages, [], 'invalid CRC must not publish')
            deadline = time.monotonic() + 10.0
            while time.monotonic() < deadline and not messages:
                sender.sendto(GOLDEN, ('127.0.0.1', PORT))
                rclpy.spin_once(node, timeout_sec=0.1)

        self.assertTrue(messages, 'no Imu received from OIMU/1 packet')
        msg = messages[0]
        self.assertEqual(msg.header.frame_id, 'test_imu')
        self.assertGreater(msg.header.stamp.sec, 1600000000)
        self.assertLess(msg.header.stamp.sec, 2147483647)
        self.assertEqual(msg.orientation_covariance[0], -1.0)
        self.assertEqual((msg.linear_acceleration.x, msg.linear_acceleration.y,
                          msg.linear_acceleration.z), (1.0, -2.5, 3.25))
        self.assertEqual((msg.angular_velocity.x, msg.angular_velocity.y,
                          msg.angular_velocity.z), (-4.0, 5.5, -6.75))
