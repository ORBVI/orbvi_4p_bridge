"""Explicit finite JPEG/OIMG bridge with subscriber-gated one-shot sessions."""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    config = os.path.join(
        get_package_share_directory('orbvi_4p_bridge'),
        'config',
        'image_jpeg.yaml',
    )
    return LaunchDescription([
        DeclareLaunchArgument('host', default_value='',
                              description='Board IPv4 address; required'),
        DeclareLaunchArgument('base_port', default_value='5610'),
        DeclareLaunchArgument('camera_mask', default_value='15'),
        DeclareLaunchArgument('frames_per_camera', default_value='10'),
        DeclareLaunchArgument('idle_timeout_sec', default_value='30.0'),
        DeclareLaunchArgument('publish_queue_depth', default_value='16'),
        DeclareLaunchArgument('drain_min_sec', default_value='3.0'),
        DeclareLaunchArgument('drain_timeout_sec', default_value='12.0'),
        DeclareLaunchArgument('topic_namespace', default_value='/orbvi/raw'),
        DeclareLaunchArgument('frame_id_prefix', default_value='orbvi_camera_'),
        Node(
            package='orbvi_4p_bridge',
            executable='orbvi_image_stream_node',
            name='orbvi_image_stream',
            output='screen',
            parameters=[config, {
                'host': LaunchConfiguration('host'),
                'base_port': ParameterValue(LaunchConfiguration('base_port'), value_type=int),
                'camera_mask': ParameterValue(LaunchConfiguration('camera_mask'), value_type=int),
                'frames_per_camera': ParameterValue(
                    LaunchConfiguration('frames_per_camera'), value_type=int),
                'idle_timeout_sec': ParameterValue(
                    LaunchConfiguration('idle_timeout_sec'), value_type=float),
                'publish_queue_depth': ParameterValue(
                    LaunchConfiguration('publish_queue_depth'), value_type=int),
                'drain_min_sec': ParameterValue(
                    LaunchConfiguration('drain_min_sec'), value_type=float),
                'drain_timeout_sec': ParameterValue(
                    LaunchConfiguration('drain_timeout_sec'), value_type=float),
                'topic_namespace': LaunchConfiguration('topic_namespace'),
                'frame_id_prefix': LaunchConfiguration('frame_id_prefix'),
            }],
        ),
    ])
