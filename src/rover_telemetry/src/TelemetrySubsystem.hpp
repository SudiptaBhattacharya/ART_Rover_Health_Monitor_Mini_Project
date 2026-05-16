// generate values(random) for project evaluation
// battery, temperature, distance
#pragma once
#include <string>

struct TelemetryData {
  int battery;
  double temperature;  // celsius
  double obstacle_distance;
};

class TelemetrySubsystem {
 private:
  int battery_;
  double temperature_;
  double obstacle_distance_;

 public:
  // constructor
  TelemetrySubsystem();

  TelemetryData generateTelemetry();
  std::string formatTelemetry(const TelemetryData& data) const;
};