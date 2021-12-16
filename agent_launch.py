#https://issueexplorer.com/issue/micro-ROS/micro-ROS-Agent/42
#https://answers.ros.org/question/318416/add-an-environment-variable-in-executeprocess/

from launch import LaunchDescription
from launch_ros.actions import Node
#from launch.actions import ExecuteProcess

def generate_launch_description():
    """
    uros_agent_process = ExecuteProcess(
             cmd=['ros2', 'run', 'micro_ros_agent', 'micro_ros_agent', 'serial', '--dev', '/dev/ttyUSB0'],
             output='screen'
    )
    """
    return LaunchDescription([
        #uros_agent_process
        Node(
            package='micro_ros_agent',
            executable='micro_ros_agent',
            name='micro_ros_agent',
            output='screen',
            emulate_tty=True,
            arguments={'serial', '--dev', '/dev/ttyUSB0'}
        ),
    ])
