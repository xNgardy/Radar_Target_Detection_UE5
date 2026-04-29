# Radar Target Detection - Unreal Engine C++

A small Unreal Engine C++ simulation that detects and classifies nearby moving targets using a radar-style scanning system.

## Project Goal

This project demonstrates a simple defense-oriented radar simulation built with Unreal Engine and C++.

The radar actor scans the environment, detects target actors within a configurable radius, calculates their distance, and classifies them as friendly, hostile, or unknown.

## Features

- Unreal Engine C++ actor-based architecture
- Radar-style target scanning
- Moving target actors
- Distance calculation between radar and targets
- Friendly / hostile / unknown target classification
- Hostile target warning message
- On-screen debug messages
- Debug radar range visualization
- Detection lines between radar and detected targets
- Rotating radar scanner visual

## Technologies

- Unreal Engine 5
- C++
- Visual Studio 2022
- Git / GitHub

## Main Classes

### `ATargetActor`

Represents a detectable target in the scene.

Properties:

- `TargetId`
- `TargetType`
- `bIsActiveTarget`
- `bCanMove`
- `MovementSpeed`
- `MovementRange`
- `MovementDirection`

### `ARadarActor`

Scans the scene for target actors.

Properties:

- `ScanRadius`
- `ScanInterval`
- `bDrawDebugSphere`
- `RotationSpeed`

## How It Works

1. The radar actor starts a timer when the game begins.
2. Every scan interval, it searches for all `ATargetActor` instances in the level.
3. It calculates the distance between itself and each target.
4. If a target is inside the scan radius, it displays detection information.
5. If a hostile target is detected, a warning message is shown.
6. Moving targets can enter and leave the radar range dynamically.
7. Debug visuals show the radar range and detection lines.

## Demo Level

The demo level contains:

- One rotating radar actor
- Multiple moving target actors
- Color-coded target classification
- Dynamic radar detection
- Hostile warning feedback

Target colors:

- Red: Hostile
- Green: Friendly
- Yellow: Unknown

## Status

Prototype completed.