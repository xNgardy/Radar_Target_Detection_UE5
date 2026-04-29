# Radar Target Detection - Unreal Engine C++

This is a small Unreal Engine 5 project where I built a simple radar-style target detection system using C++.

The main idea is to simulate a radar that scans its surroundings, detects moving targets, classifies them, and gives feedback when a hostile target is found.

I made this project to practice Unreal Engine C++ and to build a small defense-oriented simulation prototype.

## Features

- Radar actor written in C++
- Moving target actors
- Friendly / hostile / unknown target types
- Distance calculation between the radar and targets
- Target tracking states:
  - target entered radar range
  - target is being tracked
  - target lost
- Threat level calculation based on target type and distance
- Hostile target warning message
- Alarm sound when a hostile target is detected
- Directional radar sweep using angle checks
- Debug visualization for:
  - radar range
  - sweep direction
  - sweep angle
  - detected target lines
- Rotating radar scanner visual

## Demo

The demo level contains one radar actor and multiple moving targets.

Each target has a type:

- Red: Hostile
- Green: Friendly
- Yellow: Unknown

The radar rotates during play mode and scans only the area inside its sweep angle. When a target enters the sweep area, the radar starts tracking it. If the target leaves the sweep area or range, it is marked as lost.

When a hostile target is detected, the system shows a warning message and plays an alarm sound.

## Main Classes

### `ATargetActor`

Represents a target that can be detected by the radar.

Main properties:

- `TargetId`
- `TargetType`
- `bIsActiveTarget`
- `bCanMove`
- `MovementSpeed`
- `MovementRange`
- `MovementDirection`

### `ARadarActor`

Handles radar scanning, target detection, tracking, threat evaluation, and debug visualization.

Main properties:

- `ScanRadius`
- `ScanInterval`
- `bUseDirectionalSweep`
- `SweepAngleDegrees`
- `RotationSpeed`
- `bDrawSweepDebug`
- `HostileAlarmSound`

## How It Works

1. The radar starts a scan timer when the game begins.
2. At each scan interval, it checks all `ATargetActor` instances in the level.
3. It calculates the distance between the radar and each target.
4. If directional sweep is enabled, it also checks whether the target is inside the current sweep angle.
5. If a target is detected for the first time, it shows an entered-range message.
6. If the target stays visible, it continues tracking it.
7. If the target is no longer detected, it shows a target-lost message.
8. Hostile targets trigger a warning message and an alarm sound.
9. Debug lines show the radar range, sweep area, and detected targets.

## Technologies

- Unreal Engine 5
- C++
- Visual Studio 2022
- Git / GitHub

## Notes

This is a prototype project. The focus is on implementing the radar logic in C++ and making the detection behavior easy to understand visually.

Possible future improvements:

- Better UI instead of debug messages
- Target speed estimation
- Track IDs for detected targets
- More realistic radar visuals
- Detection filtering based on target size or type