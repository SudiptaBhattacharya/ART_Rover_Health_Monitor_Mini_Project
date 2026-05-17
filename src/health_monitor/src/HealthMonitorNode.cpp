#include "HealthMonitorNode.hpp"

#include <iomanip>
#include <sstream>

namespace {
const std::string RESET  = "\033[0m";
const std::string RED    = "\033[31m";
const std::string GREEN  = "\033[32m";
const std::string YELLOW = "\033[33m";

std::string colourForStatus(const std::string& status) {
    if (status == "OK") {
        return GREEN;
    }
    if (status == "WARNING") {
        return YELLOW;
    }
    if (status == "CRITICAL") {
        return RED;
    }
    return RESET;
}
}

HealthMonitorNode::HealthMonitorNode()
    : Node("health_monitor_node"), previous_status_("UNKNOWN") {
    this->declare_parameter("battery_warning_threshold", 40);
    this->declare_parameter("battery_critical_threshold", 20);
    this->declare_parameter("temperature_warning_threshold", 60.0);
    this->declare_parameter("temperature_critical_threshold", 75.0);
    this->declare_parameter("distance_warning_threshold", 1.0);
    this->declare_parameter("distance_critical_threshold", 0.5);

    subscription_ = this->create_subscription<std_msgs::msg::String>(
        "rover_telemetry",
        10,
        std::bind(&HealthMonitorNode::telemetryCallback, this, std::placeholders::_1)
    );

    alert_publisher_ = this->create_publisher<std_msgs::msg::String>("rover_alerts", 10);

    RCLCPP_INFO(this->get_logger(), "Health monitor node started.");
}

HealthThresholds HealthMonitorNode::loadThresholds() const {
    HealthThresholds thresholds{};

    thresholds.battery_warning_threshold =
        this->get_parameter("battery_warning_threshold").as_int();
    thresholds.battery_critical_threshold =
        this->get_parameter("battery_critical_threshold").as_int();

    thresholds.temperature_warning_threshold =
        this->get_parameter("temperature_warning_threshold").as_double();
    thresholds.temperature_critical_threshold =
        this->get_parameter("temperature_critical_threshold").as_double();

    thresholds.distance_warning_threshold =
        this->get_parameter("distance_warning_threshold").as_double();
    thresholds.distance_critical_threshold =
        this->get_parameter("distance_critical_threshold").as_double();

    return thresholds;
}

//better style
void HealthMonitorNode::printDashboard(const ParsedTelemetryData& data, const HealthResult& result) const {
    const std::string colour = colourForStatus(result.status);

    std::ostringstream out;
    out << "\n==================================================\n"
        << "              Rover Health Dashboard              \n"
        << "==================================================\n"
        << " Mode:        " << data.mode << "\n"
        << " Battery:     " << data.battery << "%\n"
        << " Temperature: " << std::fixed << std::setprecision(1) << data.temperature << " C\n"
        << " Distance:    " << std::fixed << std::setprecision(1) << data.obstacle_distance << " m\n"
        << " Status:      " << colour << result.status << RESET << "\n"
        << " Reason:      " << result.reason << "\n"
        << "==================================================";

    RCLCPP_INFO(this->get_logger(), "%s", out.str().c_str());
}

void HealthMonitorNode::telemetryCallback(const std_msgs::msg::String::SharedPtr msg) {
    const ParsedTelemetryData data = subsystem_.parseTelemetry(msg->data);
    const HealthThresholds thresholds = loadThresholds();
    const HealthResult result = subsystem_.assessHealth(data, thresholds);

    if (result.status != previous_status_) {
        RCLCPP_WARN(
            this->get_logger(),
            "STATE CHANGE: %s -> %s",
            previous_status_.c_str(),
            result.status.c_str()
        );

        std_msgs::msg::String alert_msg;
        alert_msg.data =
            "STATE CHANGE: " + previous_status_ + " -> " + result.status +
            " | Reason: " + result.reason +
            " | Mode: " + data.mode;
        alert_publisher_->publish(alert_msg);

        previous_status_ = result.status;
    }

    if (result.status == "WARNING" || result.status == "CRITICAL") {
        std_msgs::msg::String alert_msg;
        alert_msg.data =
            "ALERT | Status: " + result.status +
            " | Reason: " + result.reason +
            " | Mode: " + data.mode;
        alert_publisher_->publish(alert_msg);
    }

    printDashboard(data, result);
}