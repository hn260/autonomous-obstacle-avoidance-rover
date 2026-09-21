from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    pkg_share = FindPackageShare("rover_description")

    world = PathJoinSubstitution([
        pkg_share,
        "worlds",
        "rover_world.sdf"
    ])

    robot_description = Command([
        FindExecutable(name="xacro"),
        " ",
        PathJoinSubstitution([
            pkg_share,
            "urdf",
            "rover.urdf.xacro"
        ])
    ])

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([
                FindPackageShare("ros_gz_sim"),
                "launch",
                "gz_sim.launch.py"
            ])
        ]),
        launch_arguments={
            "gz_args": ["-r ", world]
        }.items()
    )

    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        name="robot_state_publisher",
        output="screen",
        parameters=[{
            "robot_description": robot_description,
            "use_sim_time": True
        }]
    )

    spawn_robot = Node(
        package="ros_gz_sim",
        executable="create",
        arguments=[
            "-topic", "robot_description",
            "-name", "autonomous_rover",
            "-x", "0",
            "-y", "0",
            "-z", "0.20"
        ],
        output="screen"
    )

    bridge = Node(
        package="ros_gz_bridge",
        executable="parameter_bridge",
        name="ros_gz_bridge",
        output="screen",
        parameters=[{
            "config_file": PathJoinSubstitution([
                pkg_share,
                "config",
                "bridge.yaml"
            ])
        }]
    )

    return LaunchDescription([
        gazebo,
        robot_state_publisher,
        spawn_robot,
        bridge
    ])
