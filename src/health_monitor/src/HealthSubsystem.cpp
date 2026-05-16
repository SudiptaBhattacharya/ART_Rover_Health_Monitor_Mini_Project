#include "HealthSubsystem.hpp"

#include <sstream>
#include <stdexcept>
#include <string>

ParsedTelemetryData HealthSubsystem::parseTelemetry(const std::string& telemetry) const {
    ParsedTelemetryData data{0, 0.0, 0.0};

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
        }
    }

    return data;
}

std::string HealthSubsystem::assessHealth(const ParsedTelemetryData& data) const {
    if (data.battery < 20 || data.temperature > 75.0 || data.obstacle_distance < 0.5) {
        return "CRITICAL";
    }

    if (data.battery < 40 || data.temperature > 60.0 || data.obstacle_distance < 1.0) {
        return "WARNING";
    }

    return "OK";
}