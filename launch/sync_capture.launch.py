"""Receive four externally triggered cameras and the IMU."""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    board = LaunchConfiguration('board_ip')
    return LaunchDescription([
        DeclareLaunchArgument('board_ip'),
        DeclareLaunchArgument('rtsp_url', default_value=['rtsp://', board, ':554/live/PRR']),
        DeclareLaunchArgument('rtsp_url_1', default_value=['rtsp://', board, ':555/live/PRR']),
        DeclareLaunchArgument('rtsp_url_2', default_value=['rtsp://', board, ':556/live/PRR']),
        DeclareLaunchArgument('rtsp_url_3', default_value=['rtsp://', board, ':557/live/PRR']),
        DeclareLaunchArgument('imu_bind_address', default_value='0.0.0.0'),
        DeclareLaunchArgument('imu_port', default_value='5600'),
        Node(
            package='orbvi_4p_bridge', executable='orbvi_bridge_node',
            name='orbvi_4p_bridge', output='screen', parameters=[{
                'runtime_mode': 'rtsp',
                'camera_count': 4,
                'image_format': 'bgr8',
                'rtsp_url': LaunchConfiguration('rtsp_url'),
                'rtsp_url_1': LaunchConfiguration('rtsp_url_1'),
                'rtsp_url_2': LaunchConfiguration('rtsp_url_2'),
                'rtsp_url_3': LaunchConfiguration('rtsp_url_3'),
                'imu_enabled': True,
                'imu_bind_address': LaunchConfiguration('imu_bind_address'),
                'imu_port': LaunchConfiguration('imu_port'),
                'timestamp_source': 'board',
                'publish_capture_metadata': True,
                'subscription_queue_capacity': 16,
                'imu_subscription_queue_capacity': 2048,
                'publish_queue_depth': 32,
                'imu_publish_queue_depth': 2048,
                'publish_reliability': 'reliable',
            }],
        ),
    ])
