#pragma once

#include "TelemetrySubsystem.hpp"

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class TelemetryNode : public rclcpp::Node {
public:
    TelemetryNode();

private:
    void publishTelemetry();
    void missionEventCallback(const std_msgs::msg::String::SharedPtr msg);

    TelemetrySubsystem subsystem_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr mission_event_subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
};