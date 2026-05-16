#include "HealthMonitorNode.hpp"

HealthMonitorNode::HealthMonitorNode() : Node("health_monitor_node") {
    subscription_ = this->create_subscription<std_msgs::msg::String>(
        "rover_telemetry",
        10,
        std::bind(&HealthMonitorNode::telemetryCallback, this, std::placeholders::_1)
    );

    RCLCPP_INFO(this->get_logger(), "Health monitor node started.");
}

void HealthMonitorNode::telemetryCallback(const std_msgs::msg::String::SharedPtr msg) {
    const ParsedTelemetryData data = subsystem_.parseTelemetry(msg->data);
    const std::string health = subsystem_.assessHealth(data);

    RCLCPP_INFO(
        this->get_logger(),
        "Received telemetry: %s | Health status: %s",
        msg->data.c_str(),
        health.c_str()
    );
}