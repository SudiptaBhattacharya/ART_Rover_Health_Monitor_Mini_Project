#include "TelemetryNode.hpp"

#include <chrono>

using namespace std::chrono_literals;

TelemetryNode::TelemetryNode() : Node("rover_telemetry_node") {
    publisher_ = this->create_publisher<std_msgs::msg::String>("rover_telemetry", 10);

    mission_event_subscription_ = this->create_subscription<std_msgs::msg::String>(
        "mission_event",
        10,
        std::bind(&TelemetryNode::missionEventCallback, this, std::placeholders::_1)
    );

    timer_ = this->create_wall_timer(
        1s,
        std::bind(&TelemetryNode::publishTelemetry, this)
    );

    RCLCPP_INFO(this->get_logger(), "Telemetry node started.");
    RCLCPP_INFO(this->get_logger(), "Listening for mission events on /mission_event");
}

void TelemetryNode::missionEventCallback(const std_msgs::msg::String::SharedPtr msg) {
    subsystem_.setMissionPhase(msg->data);

    RCLCPP_WARN(
        this->get_logger(),
        "MISSION EVENT RECEIVED: switching to phase triggered by '%s'",
        msg->data.c_str()
    );
}

void TelemetryNode::publishTelemetry() {
    const TelemetryData data = subsystem_.generateTelemetry();

    std_msgs::msg::String msg;
    msg.data = subsystem_.formatTelemetry(data);

    publisher_->publish(msg);

    RCLCPP_INFO(this->get_logger(), "Published telemetry: %s", msg.data.c_str());
}