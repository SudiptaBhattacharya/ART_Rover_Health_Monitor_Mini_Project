// generate values(random) for project evaluation
// battery, temperature, distance
//get mission phase actions
#pragma once

#include <string>

struct TelemetryData {
    int battery;
    double temperature;
    double obstacle_distance;
    std::string mode;
    std::string mission_phase;
};

class TelemetrySubsystem {
public:
    TelemetrySubsystem();

    TelemetryData generateTelemetry();
    std::string formatTelemetry(const TelemetryData& data) const;

private:
    int phase_index_;
    int step_in_phase_;

    void advancePhaseIfNeeded();
};