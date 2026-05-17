# ART Rover Health Monitor Mini Project - Rover Health Monitor + Action Controller

## Overview

This project is a ROS 2 Humble mini project built in C++ to simulate rover telemetry, monitor rover health in real time, and publish safety-related alerts and commands
I also added live mission event injection so an operator can trigger scenarios in real time during the demo.

The system is designed around a rover health monitoring concept:
- one package simulates rover telemetry during different mission phases
- one package monitors incoming telemetry, evaluates rover health, and determines appropriate safety actions


## Package roles

# rover_telemetry:
This package simulates rover telemetry data and publishes it to the ROS 2 network.

Its subsystem generates mission-based telemetry across different mission phases:

STARTUP_CHECK
CRUISE
ROUGH_TERRAIN
OBSTACLE_ENCOUNTER
LOW_POWER
EMERGENCY
RECOVERY

The telemetry includes:

battery percentage
temperature
obstacle distance
rover mode
mission phase

# health_monitor:
This package subscribes to telemetry, assesses rover health, displays a formatted dashboard, logs state changes, and publishes alerts and command recommendations.

It determines:

overall health status (OK, WARNING, CRITICAL)
reason for the status
recommended rover action

Example actions include:

CONTINUE_NORMAL
REDUCE_SPEED
LIMIT_MOTOR_LOAD
PREPARE_RETURN
STOP_IMMEDIATELY
ENTER_SAFE_MODE
SHUTDOWN_NONESSENTIAL_SYSTEMS

## ROS 2 Topics
/rover_telemetry

Published by: rover_telemetry
Subscribed by: health_monitor

Carries simulated rover telemetry data.

/rover_alerts

Published by: health_monitor

Carries warning/critical alerts and state-change notifications.

/rover_commands

Published by: health_monitor

Carries recommended rover actions based on current telemetry and health assessment.

/mission_events:
Supports live operator-injected mission events

## Run Instructions:

# Option 1: Run Nodes manually

# Terminal 1:
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 run health_monitor health_monitor

# Terminal 2:
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 run health_monitor health_monitor

# Terminals for alerts and commands:
ros2 topic echo /rover_alerts

ros2 topic echo /rover_commands

# Option 2: Launch File
ros2 launch health_monitor rover_health.launch.py


## System Logic
The telemetry subsystem follows a mission profile, which makes the monitoring system behave more realistically:

Startup check
The rover begins with healthy conditions.
Cruise
Battery slowly decreases and temperature rises slightly.
Rough terrain
Temperature rises faster and the rover begins experiencing higher load.
Obstacle encounter
Obstacle distance drops, causing warnings or critical actions.
Low power
Battery drops into warning or critical ranges.
Emergency
Critical telemetry values trigger safety actions.
Recovery
Conditions improve and the rover begins returning toward safer operation.

This allows the dashboard and topics to tell a mission-style story rather than just printing static values

## Live Mission Event Injection
The telemetry simulator supports live operator-injected mission events through the `/mission_event` topic. This allows the system to be tested interactively during runtime without changing the code.

Supported mission events include:
- `startup`
- `normal`
- `cruise`
- `rough`
- `obstacle`
- `low_power`
- `emergency`
- `recovery`
- `resume_auto`

These events can be injected from another terminal while the system is running.

### Example Commands

Trigger obstacle encounter:

- ros2 topic pub --once /mission_event std_msgs/msg/String "{data: 'obstacle'}"

Trigger rough terrain:

- ros2 topic pub --once /mission_event std_msgs/msg/String "{data: 'rough'}"

Trigger low-power condition:

- ros2 topic pub --once /mission_event std_msgs/msg/String "{data: 'low_power'}"

Trigger emergency state:

- ros2 topic pub --once /mission_event std_msgs/msg/String "{data: 'emergency'}"

Trigger recovery:

- ros2 topic pub --once /mission_event std_msgs/msg/String "{data: 'recovery'}"

Return to automatic mission flow:

- ros2 topic pub --once /mission_event std_msgs/msg/String "{data: 'resume_auto'}"

This interaction makes the project more representative of a real rover control environment, where an operator or external subsystem may inject mission events in real time.

## Features

### Core Features
- One ROS 2 workspace containing two ROS 2 packages
- One node in each package
- Topic-based communication between packages
- Plain C++ subsystem logic separated from ROS 2 node logic
- Simulated telemetry publishing
- Real-time health assessment
- Dashboard-style terminal output
- State-change logging
- Alert publishing
- Safety command publishing

### Extensions Implemented
- More complex subsystem-specific functionality
- Node parameters for health thresholds
- Launch file to run the system and set parameters
