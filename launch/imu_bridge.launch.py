"""Explicit UDP-only IMU bridge; does not start a camera or board service."""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument('imu_bind_address', default_value='0.0.0.0'),
        DeclareLaunchArgument('imu_source_address', default_value=''),
        DeclareLaunchArgument('imu_port', default_value='5600'),
        DeclareLaunchArgument('imu_frame_id', default_value='orbvi_imu'),
        Node(package='orbvi_4p_bridge', executable='orbvi_bridge_node',
             name='orbvi_4p_bridge', output='screen', parameters=[{
                 'runtime_mode': 'udp_imu', 'imu_enabled': True,
                 'imu_bind_address': LaunchConfiguration('imu_bind_address'),
                 'imu_source_address': LaunchConfiguration('imu_source_address'),
                 'imu_port': ParameterValue(LaunchConfiguration('imu_port'), value_type=int),
                 'imu_frame_id': LaunchConfiguration('imu_frame_id'),
             }]),
    ])
