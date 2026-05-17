// generate values(random) for project evaluation
// battery, temperature, distance
#pragma once
#include <string>

struct TelemetryData {
  int battery;
  double temperature;  // celsius
  double obstacle_distance;
  std::string mode;
};

class TelemetrySubsystem {
 private:
  int step_;

 public:
  // constructor
  TelemetrySubsystem();

  TelemetryData generateTelemetry();
  std::string formatTelemetry(const TelemetryData& data) const;
};