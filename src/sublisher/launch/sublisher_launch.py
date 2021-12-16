from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package="sublisher",
            namespace="test1",
            executable="param_pub",
            name="custom_param_pub",
            parameters=[
                {"my_topic": "/position/SAIL_ANGLE"},
                {"my_start": 5.0},
                {"my_turn": True}  # Start by adding
            ]
        ),
        Node(
            package="sublisher",
            namespace="test1",
            executable="param_pub",
            name="custom_param_pub",
            parameters=[
                {"my_topic": "/rudder/ANGLE"},
                {"my_start": -5.0},
                {"my_turn": False}  # Start by subtracting
            ]
        ),
    ])
