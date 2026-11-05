import time
import threading
import unittest

import launch
import launch_ros.actions
import launch_testing.actions
import rclpy
from rclpy.qos import DurabilityPolicy, QoSProfile, ReliabilityPolicy
from sensor_msgs.msg import Image


def generate_test_description():
    node = launch_ros.actions.Node(
        package='orbvi_4p_bridge',
        executable='orbvi_bridge_node',
        name='orbvi_4p_bridge',
        output='screen',
        parameters=[{
            'runtime_mode': 'fake',
            'camera_count': 4,
            'frames_per_camera': 300,
            'emit_interval_us': 100000,
            'frame_width': 64,
            'frame_height': 48,
            'frame_stride': 0,
            'callback_queue_capacity': 8,
            'subscription_queue_capacity': 8,
            'publish_queue_depth': 5,
            'topic_namespace': '/orbvi/raw',
            'frame_id_prefix': 'orbvi_camera_',
        }],
    )
    return launch.LaunchDescription([
        node,
        launch_testing.actions.ReadyToTest(),
    ]), {'bridge_node': node}


class TestFourCameraFakeImages(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        rclpy.init()

    @classmethod
    def tearDownClass(cls):
        rclpy.shutdown()

    def test_each_camera_has_one_mono8_image(self):
        node = rclpy.create_node('orbvi_4p_bridge_test')
        self.addCleanup(node.destroy_node)
        qos = QoSProfile(
            depth=5,
            reliability=ReliabilityPolicy.BEST_EFFORT,
            durability=DurabilityPolicy.VOLATILE,
        )

        topics = [f'/orbvi/raw/camera_{index}/image' for index in range(4)]
        events = [threading.Event() for _ in range(4)]
        messages = [None] * 4
        subscriptions = []

        for index in range(4):
            def callback(message, camera_index=index):
                messages[camera_index] = message
                events[camera_index].set()

            subscriptions.append(
                node.create_subscription(Image, topics[index], callback, qos)
            )

        graph_deadline = time.monotonic() + 10.0
        while time.monotonic() < graph_deadline:
            rclpy.spin_once(node, timeout_sec=0.1)
            if all(node.count_publishers(topic) > 0 for topic in topics):
                break

        publisher_counts = {
            topic: node.count_publishers(topic) for topic in topics
        }
        topic_types = dict(node.get_topic_names_and_types())
        self.assertTrue(
            all(count > 0 for count in publisher_counts.values()),
            'ROS graph discovery failed before image wait: '
            f'publisher_counts={publisher_counts}, topic_types={topic_types}',
        )

        deadline = time.monotonic() + 10.0
        while time.monotonic() < deadline:
            rclpy.spin_once(node, timeout_sec=0.1)
            if all(event.is_set() for event in events):
                break

        for index, event in enumerate(events):
            self.assertTrue(
                event.is_set(),
                f'no image received on {topics[index]}; '
                f'publisher_counts={publisher_counts}, topic_types={topic_types}',
            )

        for index, message in enumerate(messages):
            self.assertIsNotNone(message)
            self.assertEqual(message.encoding, 'mono8')
            self.assertEqual(message.header.frame_id, f'orbvi_camera_{index}')
            self.assertEqual(message.width, 64)
            self.assertEqual(message.height, 48)
            self.assertEqual(message.step, 64)
            self.assertEqual(len(message.data), 64 * 48)

        for topic in topics:
            self.assertIn(topic, topic_types)
            self.assertIn('sensor_msgs/msg/Image', topic_types[topic])
        self.assertNotIn('/orbvi/raw/camera_0/metadata', topic_types)
