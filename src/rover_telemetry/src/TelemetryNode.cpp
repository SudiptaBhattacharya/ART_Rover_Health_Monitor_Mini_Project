#include "TelemetryNode.hpp"

#include <chrono>

using namespace std::chrono_literals;

TelemetryNode::TelemetryNode() : Node("rover_telemetry_node") {
    publisher_ = this->create_publisher<std_msgs::msg::String>("rover_telemetry", 10);

    timer_ = this->create_wall_timer(
        1s,
        std::bind(&TelemetryNode::publishTelemetry, this)
    );

    RCLCPP_INFO(this->get_logger(), "Telemetry node started.");
}

void TelemetryNode::publishTelemetry() {
    const TelemetryData data = subsystem_.generateTelemetry();

    std_msgs::msg::String msg;
    msg.data = subsystem_.formatTelemetry(data);

    publisher_->publish(msg);

    RCLCPP_INFO(this->get_logger(), "Published telemetry: %s", msg.data.c_str());
}