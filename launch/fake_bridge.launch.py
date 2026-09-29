from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

import os


def generate_launch_description():
    config = os.path.join(
        get_package_share_directory('orbvi_4p_bridge'),
        'config',
        'fake_bridge.yaml',
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'runtime_mode',
            default_value='unsupported',
            description=(
                'Must be explicitly set to fake to allow synthetic frames; '
                'the default is fail-closed.'
            ),
        ),
        Node(
            package='orbvi_4p_bridge',
            executable='orbvi_bridge_node',
            name='orbvi_4p_bridge',
            output='screen',
            parameters=[
                config,
                {'runtime_mode': LaunchConfiguration('runtime_mode')},
            ],
        ),
    ])