from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    telemetry_node = Node(
        package="rover_telemetry",
        executable="rover_telemetry",
        name="rover_telemetry_node",
        output="screen"
    )

    health_monitor_node = Node(
        package="health_monitor",
        executable="health_monitor",
        name="health_monitor_node",
        output="screen",
        parameters=[
            {
                "battery_warning_threshold": 40,
                "battery_critical_threshold": 20,
                "temperature_warning_threshold": 60.0,
                "temperature_critical_threshold": 75.0,
                "distance_warning_threshold": 1.0,
                "distance_critical_threshold": 0.5
            }
        ]
    )

    return LaunchDescription([
        telemetry_node,
        health_monitor_node
    ])