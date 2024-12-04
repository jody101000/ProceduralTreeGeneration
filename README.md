# Procedural Tree Generation using LSystem and Space Colonization Algorithms

A 3D procedural tree generation project.

## Overview

This project demonstrates:
- 3D graphics rendering using OpenGL
- Procedural tree generation using recursive algorithms (in `tree.cpp`)
- Interactive camera system with multiple control modes
- Basic lighting and shading

## Important Notes for Contributors

### CMake Integration
- If Visual Studio prompts you with "CMake Integration" dialog asking to "enable Visual Studio's CMake support with this workspace", choose **"Do not enable"**
- This project uses standard Visual Studio C++ project structure, not CMake
- Enabling CMake support may cause build configuration issues

## Dependencies

The project uses the following libraries:
- GLFW 3.3: Window management and OpenGL context
- GLAD: OpenGL loader
- GLM: Mathematics library
- ImGui: Immediate mode GUI library for parameter controls and interface
- OpenMP: Used for parallel processing in tree growth calculations

## Building and Running

1. Set the Solution Configuration to "Debug" or "Release"
2. Build Solution (F7 or ctrl+B)
3. Run (F5)

## Camera Controls

The visualization features an interactive camera system with the following controls:

### Orbital Controls
- `W/S`: Rotate camera down/up around focus
- `A/D`: Rotate camera left/right around focus

### Direct Movement
- `Page Up/Down`: Move camera up/down vertically
- `Left/Right`: Move camera left/right horizontally

### Additional Controls
- `Space`: Reset camera to default position and enable auto-rotation
- `Mouse Scroll`: Zoom in/out during auto rotation
- Any movement key automatically disables auto-rotation
