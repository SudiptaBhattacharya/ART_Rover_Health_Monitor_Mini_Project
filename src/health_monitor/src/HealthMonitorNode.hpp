#pragma once

#include "HealthSubsystem.hpp"

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class HealthMonitorNode : public rclcpp::Node {
public:
    HealthMonitorNode();

private:
    void telemetryCallback(const std_msgs::msg::String::SharedPtr msg);

    HealthSubsystem subsystem_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};