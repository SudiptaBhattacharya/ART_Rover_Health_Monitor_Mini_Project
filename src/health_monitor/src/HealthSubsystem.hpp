#pragma once

#include <string>
#include <vector>

struct ParsedTelemetryData {
    int battery;
    double temperature;
    double obstacle_distance;
    std::string mode;
    std::string mission_phase;
};

struct HealthResult {
    std::string status;
    std::vector<std::string> reasons;
    std::string action;
};

struct HealthThresholds {
    int battery_warning_threshold;
    int battery_critical_threshold;
    double temperature_warning_threshold;
    double temperature_critical_threshold;
    double distance_warning_threshold;
    double distance_critical_threshold;
};

class HealthSubsystem {
public:
    ParsedTelemetryData parseTelemetry(const std::string& telemetry) const;
    HealthResult assessHealth(const ParsedTelemetryData& data, const HealthThresholds& thresholds) const;
};