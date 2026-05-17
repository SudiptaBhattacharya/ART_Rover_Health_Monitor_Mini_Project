#include "TelemetrySubsystem.hpp"

#include <sstream>

TelemetrySubsystem::TelemetrySubsystem() : step_(0) {}

TelemetryData TelemetrySubsystem::generateTelemetry() {
    TelemetryData data{};

    int phase = step_ % 15;

    if (phase >= 0 && phase <= 2) {
        data.battery = 95 - phase * 2;
        data.temperature = 35.0 + phase * 1.5;
        data.obstacle_distance = 3.0;
        data.mode = "NORMAL";
    } else if (phase >= 3 && phase <= 5) {
        data.battery = 82 - (phase - 3) * 4;
        data.temperature = 48.0 + (phase - 3) * 5.0;
        data.obstacle_distance = 1.8 - (phase - 3) * 0.3;
        data.mode = "ROUGH_TERRAIN";
    } else if (phase >= 6 && phase <= 8) {
        data.battery = 60 - (phase - 6) * 6;
        data.temperature = 58.0 + (phase - 6) * 4.0;
        data.obstacle_distance = 0.9 - (phase - 6) * 0.2;
        data.mode = "OBSTACLE_NEAR";
    } else if (phase >= 9 && phase <= 11) {
        data.battery = 18 - (phase - 9) * 3;
        data.temperature = 76.0 + (phase - 9) * 2.0;
        data.obstacle_distance = 0.4;
        data.mode = "CRITICAL_STATE";
    } else {
        data.battery = 50 + (phase - 12) * 10;
        data.temperature = 55.0 - (phase - 12) * 6.0;
        data.obstacle_distance = 1.5 + (phase - 12) * 0.7;
        data.mode = "RECOVERY";
    }

    ++step_;
    return data;
}

std::string TelemetrySubsystem::formatTelemetry(const TelemetryData& data) const {
    std::ostringstream oss;
    oss << "battery=" << data.battery
        << ",temp=" << data.temperature
        << ",distance=" << data.obstacle_distance
        << ",mode=" << data.mode;
    return oss.str();
}