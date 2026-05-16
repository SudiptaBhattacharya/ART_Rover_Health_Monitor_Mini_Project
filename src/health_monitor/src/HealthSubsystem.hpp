#pragma once

#include <string>

struct ParsedTelemetryData {
    int battery;
    double temperature;
    double obstacle_distance;
};

class HealthSubsystem {
public:
    ParsedTelemetryData parseTelemetry(const std::string& telemetry) const;
    std::string assessHealth(const ParsedTelemetryData& data) const;
};