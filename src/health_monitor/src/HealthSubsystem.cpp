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
    std::vector<std::string> critical_reasons;
    std::vector<std::string> warning_reasons;

    if (data.obstacle_distance < thresholds.distance_critical_threshold) {
        critical_reasons.push_back("Obstacle dangerously close");
    } else if (data.obstacle_distance < thresholds.distance_warning_threshold) {
        warning_reasons.push_back("Obstacle approaching");
    }

    if (data.battery < thresholds.battery_critical_threshold) {
        critical_reasons.push_back("Battery critically low");
    } else if (data.battery < thresholds.battery_warning_threshold) {
        warning_reasons.push_back("Battery getting low");
    }

    if (data.temperature > thresholds.temperature_critical_threshold) {
        critical_reasons.push_back("Motor temperature critically high");
    } else if (data.temperature > thresholds.temperature_warning_threshold) {
        warning_reasons.push_back("Motor temperature rising");
    }

    if (!critical_reasons.empty()) {
        std::string action = "ENTER_SAFE_MODE";

        for (const auto& reason : critical_reasons) {
            if (reason == "Obstacle dangerously close") {
                action = "STOP_IMMEDIATELY";
                break;
            }
            if (reason == "Motor temperature critically high") {
                action = "SHUTDOWN_NONESSENTIAL_SYSTEMS";
            }
        }

        return {"CRITICAL", critical_reasons, action};
    }

    if (!warning_reasons.empty()) {
        std::string action = "CONTINUE_NORMAL";

        for (const auto& reason : warning_reasons) {
            if (reason == "Obstacle approaching") {
                action = "REDUCE_SPEED";
                break;
            }
            if (reason == "Motor temperature rising") {
                action = "LIMIT_MOTOR_LOAD";
            } else if (reason == "Battery getting low" && action == "CONTINUE_NORMAL") {
                action = "PREPARE_RETURN";
            }
        }

        return {"WARNING", warning_reasons, action};
    }

    return {"OK", {"All systems normal"}, "CONTINUE_NORMAL"};
}