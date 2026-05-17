#include "HealthSubsystem.hpp"

#include <sstream>
#include <string>

ParsedTelemetryData HealthSubsystem::parseTelemetry(const std::string& telemetry) const {
    ParsedTelemetryData data{0, 0.0, 0.0, "UNKNOWN", "UNKNOWN"};

    std::stringstream ss(telemetry);
    std::string token;

    while (std::getline(ss, token, ',')) {
        const auto equals_pos = token.find('=');
        if (equals_pos == std::string::npos) {
            continue;
        }

        const std::string key = token.substr(0, equals_pos);
        const std::string value = token.substr(equals_pos + 1);

        if (key == "battery") {
            data.battery = std::stoi(value);
        } else if (key == "temp") {
            data.temperature = std::stod(value);
        } else if (key == "distance") {
            data.obstacle_distance = std::stod(value);
        } else if (key == "mode") {
            data.mode = value;
        } else if (key == "phase") {
            data.mission_phase = value;
        }
    }

    return data;
}

HealthResult HealthSubsystem::assessHealth(
    const ParsedTelemetryData& data,
    const HealthThresholds& thresholds
) const {
    if (data.obstacle_distance < thresholds.distance_critical_threshold) {
        return {"CRITICAL", "Obstacle dangerously close", "STOP_IMMEDIATELY"};
    }

    if (data.battery < thresholds.battery_critical_threshold) {
        return {"CRITICAL", "Battery critically low", "ENTER_SAFE_MODE"};
    }

    if (data.temperature > thresholds.temperature_critical_threshold) {
        return {"CRITICAL", "Motor temperature critically high", "SHUTDOWN_NONESSENTIAL_SYSTEMS"};
    }

    if (data.obstacle_distance < thresholds.distance_warning_threshold) {
        return {"WARNING", "Obstacle approaching", "REDUCE_SPEED"};
    }

    if (data.temperature > thresholds.temperature_warning_threshold) {
        return {"WARNING", "Motor temperature rising", "LIMIT_MOTOR_LOAD"};
    }

    if (data.battery < thresholds.battery_warning_threshold) {
        return {"WARNING", "Battery getting low", "PREPARE_RETURN"};
    }

    return {"OK", "All systems normal", "CONTINUE_NORMAL"};
}