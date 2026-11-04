"""Standalone OCPU/1 processor-temperature bridge, independent of IMU."""

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
        'temperature.yaml',
    )
    return LaunchDescription([
        DeclareLaunchArgument('bind_address', default_value='0.0.0.0'),
        DeclareLaunchArgument('port', default_value='5620'),
        DeclareLaunchArgument('samples', default_value='10'),
        DeclareLaunchArgument('idle_timeout_sec', default_value='30.0'),
        DeclareLaunchArgument('topic', default_value='/orbvi/raw/temperature'),
        DeclareLaunchArgument('source', default_value='cpu'),
        Node(
            package='orbvi_4p_bridge',
            executable='orbvi_temperature_node',
            name='orbvi_temperature',
            output='screen',
            parameters=[config, {
                'bind_address': LaunchConfiguration('bind_address'),
                'port': ParameterValue(LaunchConfiguration('port'), value_type=int),
                'samples': ParameterValue(LaunchConfiguration('samples'), value_type=int),
                'idle_timeout_sec': ParameterValue(
                    LaunchConfiguration('idle_timeout_sec'), value_type=float),
                'topic': LaunchConfiguration('topic'),
                'source': LaunchConfiguration('source'),
            }],
        ),
    ])
