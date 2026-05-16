#include "TelemetrySubsystem.hpp"

#include <algorithm>
#include <sstream>

TelemetrySubsystem::TelemetrySubsystem()
    : battery_(100), temperature_(35.0), obstacle_distance_(3.0) {}

TelemetryData TelemetrySubsystem::generateTelemetry() {
    // Simulate changing rover values.
    battery_ = std::max(0, battery_ - 1);

    temperature_ += 1.5;
    if (temperature_ > 85.0) {
        temperature_ = 35.0;
    }

    obstacle_distance_ -= 0.2;
    if (obstacle_distance_ < 0.2) {
        obstacle_distance_ = 3.0;
    }

    return TelemetryData{battery_, temperature_, obstacle_distance_};
}
//format rover message:
std::string TelemetrySubsystem::formatTelemetry(const TelemetryData& data) const {
    std::ostringstream oss;
    oss << "battery=" << data.battery
        << ",temp=" << data.temperature
        << ",distance=" << data.obstacle_distance;
    return oss.str();
}