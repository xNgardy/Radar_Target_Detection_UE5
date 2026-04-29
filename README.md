# Radar Target Detection - Unreal Engine C++

A small Unreal Engine C++ simulation that detects and classifies nearby targets using a radar-style scanning system.

## Project Goal

This project demonstrates a simple defense-oriented radar simulation built with Unreal Engine and C++.

The radar actor scans the environment, detects target actors within a configurable radius, calculates their distance, and classifies them as friendly, hostile, or unknown.

## Features

- Unreal Engine C++ actor-based architecture
- Radar-style target scanning
- Distance calculation between radar and targets
- Friendly / hostile / unknown target classification
- On-screen debug messages
- Debug sphere and detection lines for visualization

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

### `ARadarActor`

Scans the scene for target actors.

Properties:

- `ScanRadius`
- `ScanInterval`
- `bDrawDebugSphere`

## How It Works

1. The radar actor starts a timer when the game begins.
2. Every scan interval, it searches for all `ATargetActor` instances in the level.
3. It calculates the distance between itself and each target.
4. If a target is inside the scan radius, it prints detection information on screen.
5. Debug visuals show the radar range and detected target lines.

## Demo Scenario

The level contains one radar actor and multiple target actors placed at different distances.

Each target has a type:

- Friendly
- Hostile
- Unknown

When the simulation runs, the radar detects targets within range and displays their information.

## Status

Work in progress.