"""Explicit 1-4 camera RTSP bridge; never falls back to Fake."""

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
        'rtsp_bridge.yaml',
    )
    return LaunchDescription([
        DeclareLaunchArgument('rtsp_url', default_value='',
                              description='Explicit H.264 RTSP URL for camera_index=0'),
        DeclareLaunchArgument('rtsp_url_1', default_value=''),
        DeclareLaunchArgument('rtsp_url_2', default_value=''),
        DeclareLaunchArgument('rtsp_url_3', default_value=''),
        DeclareLaunchArgument('camera_count', default_value='1'),
        DeclareLaunchArgument('imu_enabled', default_value='false'),
        DeclareLaunchArgument('imu_bind_address', default_value='0.0.0.0'),
        DeclareLaunchArgument('imu_source_address', default_value=''),
        DeclareLaunchArgument('imu_port', default_value='5600'),
        DeclareLaunchArgument('imu_frame_id', default_value='orbvi_imu'),
        Node(
            package='orbvi_4p_bridge',
            executable='orbvi_bridge_node',
            name='orbvi_4p_bridge',
            output='screen',
            parameters=[config, {
                'runtime_mode': 'rtsp',
                'camera_count': ParameterValue(LaunchConfiguration('camera_count'), value_type=int),
                'rtsp_url': LaunchConfiguration('rtsp_url'),
                'rtsp_url_1': LaunchConfiguration('rtsp_url_1'),
                'rtsp_url_2': LaunchConfiguration('rtsp_url_2'),
                'rtsp_url_3': LaunchConfiguration('rtsp_url_3'),
                'imu_enabled': ParameterValue(LaunchConfiguration('imu_enabled'), value_type=bool),
                'imu_bind_address': LaunchConfiguration('imu_bind_address'),
                'imu_source_address': LaunchConfiguration('imu_source_address'),
                'imu_port': ParameterValue(LaunchConfiguration('imu_port'), value_type=int),
                'imu_frame_id': LaunchConfiguration('imu_frame_id'),
            }],
        ),
    ])
