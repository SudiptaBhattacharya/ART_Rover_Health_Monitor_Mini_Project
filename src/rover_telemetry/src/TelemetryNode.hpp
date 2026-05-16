#pragma once

#include "TelemetrySubsystem.hpp"

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class TelemetryNode : public rclcpp::Node {
public:
    TelemetryNode();

private:
    void publishTelemetry();

    TelemetrySubsystem subsystem_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
}; 