#pragma once

#include <string>

struct ParsedTelemetryData {
    int battery;
    double temperature;
    double obstacle_distance;
    std::string mode;
};

struct HealthResult {
    std::string status;
    std::string reason;
};

class HealthSubsystem {
public:
    ParsedTelemetryData parseTelemetry(const std::string& telemetry) const;
    HealthResult assessHealth(const ParsedTelemetryData& data) const;
};