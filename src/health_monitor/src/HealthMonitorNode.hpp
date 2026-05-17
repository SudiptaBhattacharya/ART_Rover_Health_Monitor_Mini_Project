#pragma once

#include "HealthSubsystem.hpp"

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class HealthMonitorNode : public rclcpp::Node {
public:
    HealthMonitorNode();

private:
    void telemetryCallback(const std_msgs::msg::String::SharedPtr msg);
    HealthThresholds loadThresholds() const;
    void printDashboard(const ParsedTelemetryData& data, const HealthResult& result) const;

    HealthSubsystem subsystem_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr alert_publisher_;

    std::string previous_status_;
};