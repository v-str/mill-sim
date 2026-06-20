import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    pkg_dir = get_package_share_directory('client_tcp_connector')
    params_file = os.path.join(pkg_dir, 'config', 'client_tcp_connector.yaml')

    return LaunchDescription([
        Node(
            package='client_tcp_connector',
            executable='client_tcp_connector',
            name='client_tcp_connector',
            parameters=[params_file],
            output='screen',
        ),
    ])
