#include "TelemetrySubsystem.hpp"

#include <sstream>

TelemetrySubsystem::TelemetrySubsystem()
    : phase_index_(0), step_in_phase_(0), override_active_(false) {}

int TelemetrySubsystem::phaseIndexFromEvent(
    const std::string& event_name) const {
  if (event_name == "startup") {
    return 0;
  }
  if (event_name == "cruise" || event_name == "normal") {
    return 1;
  }
  if (event_name == "rough") {
    return 2;
  }
  if (event_name == "obstacle") {
    return 3;
  }
  if (event_name == "low_power") {
    return 4;
  }
  if (event_name == "emergency") {
    return 5;
  }
  if (event_name == "recovery") {
    return 6;
  }
  return phase_index_;
}

void TelemetrySubsystem::setMissionPhase(const std::string& event_name) {
  if (event_name == "resume_auto") {
    clearOverride();
    return;
  }

  phase_index_ = phaseIndexFromEvent(event_name);
  step_in_phase_ = 0;
  override_active_ = true;
}

void TelemetrySubsystem::clearOverride() {
  phase_index_ = 1;  // CRUISE
  step_in_phase_ = 0;
  override_active_ = false;
}

void TelemetrySubsystem::advancePhaseIfNeeded() {
  ++step_in_phase_;

  if (override_active_) {
    // stay in the injected phase, but loop local steps for variation
    if (step_in_phase_ >= 3) {
      step_in_phase_ = 0;
    }
    return;
  }

  if (step_in_phase_ >= 3) {
    step_in_phase_ = 0;
    phase_index_ = (phase_index_ + 1) % 7;
  }
}

TelemetryData TelemetrySubsystem::generateTelemetry() {
  TelemetryData data{};

  switch (phase_index_) {
    case 0:
      data.battery = 100 - step_in_phase_;
      data.temperature = 30.0 + step_in_phase_;
      data.obstacle_distance = 5.0;
      data.mode = "INITIALISING";
      data.mission_phase = "STARTUP_CHECK";
      break;

    case 1:
      data.battery = 96 - step_in_phase_ * 2;
      data.temperature = 34.0 + step_in_phase_ * 1.5;
      data.obstacle_distance = 4.0;
      data.mode = "CRUISING";
      data.mission_phase = "CRUISE";
      break;

    case 2:
      data.battery = 88 - step_in_phase_ * 4;
      data.temperature = 45.0 + step_in_phase_ * 5.0;
      data.obstacle_distance = 2.0 - step_in_phase_ * 0.2;
      data.mode = "ROUGH_TERRAIN";
      data.mission_phase = "ROUGH_TERRAIN";
      break;

    case 3:
      data.battery = 74 - step_in_phase_ * 3;
      data.temperature = 58.0 + step_in_phase_ * 3.0;
      data.obstacle_distance = 1.0 - step_in_phase_ * 0.3;
      data.mode = "OBSTACLE_NEAR";
      data.mission_phase = "OBSTACLE_ENCOUNTER";
      break;

    case 4:
      data.battery = 35 - step_in_phase_ * 7;
      data.temperature = 60.0 + step_in_phase_ * 2.0;
      data.obstacle_distance = 1.8;
      data.mode = "LOW_POWER";
      data.mission_phase = "LOW_POWER";
      break;

    case 5:
      data.battery = 18 - step_in_phase_ * 2;
      data.temperature = 76.0 + step_in_phase_ * 2.0;
      data.obstacle_distance = 0.4;
      data.mode = "CRITICAL_STATE";
      data.mission_phase = "EMERGENCY";
      break;

    case 6:
      data.battery = 28 + step_in_phase_ * 12;
      data.temperature = 65.0 - step_in_phase_ * 8.0;
      data.obstacle_distance = 1.5 + step_in_phase_ * 0.8;
      data.mode = "RECOVERY";
      data.mission_phase = "RECOVERY";
      break;

    default:
      data.battery = 100;
      data.temperature = 30.0;
      data.obstacle_distance = 5.0;
      data.mode = "UNKNOWN";
      data.mission_phase = "UNKNOWN";
      break;
  }

  advancePhaseIfNeeded();
  return data;
}

std::string TelemetrySubsystem::formatTelemetry(
    const TelemetryData& data) const {
  std::ostringstream oss;
  oss << "battery=" << data.battery << ",temp=" << data.temperature
      << ",distance=" << data.obstacle_distance << ",mode=" << data.mode
      << ",phase=" << data.mission_phase;
  return oss.str();
}