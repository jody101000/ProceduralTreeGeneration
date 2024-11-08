# 3D Tree Visualization (temporary)

A 3D tree visualization project using OpenGL, prepared for a Procedrual Tree Generation Project. 
Tree branch generation is still in a rough state. 

## Overview

This project demonstrates:
- 3D graphics rendering using OpenGL
- Procedural tree generation using recursive algorithms (in `tree.cpp`)
- Interactive camera system with multiple control modes
- Basic lighting and shading
- Modern C++ practices with modular design

## Important Notes for Contributors

### CMake Integration
- If Visual Studio prompts you with "CMake Integration" dialog asking to "enable Visual Studio's CMake support with this workspace", choose **"Do not enable"**
- This project uses standard Visual Studio C++ project structure, not CMake
- Enabling CMake support may cause build configuration issues

## Dependencies

The project uses the following libraries:
- GLFW 3.3 (Window management and OpenGL context)
- GLAD (OpenGL loader)
- GLM (Mathematics library)

All the dependencies are stored in `external` folder.

## Building and Running

1. Set the Solution Configuration to "Debug" or "Release"
2. Build Solution (F7 or ctrl+B)
3. Run (F5)

## Camera Controls

The visualization features an interactive camera system with the following controls:

### Orbital Controls
- `W/S`: Adjust camera height up/down
- `A/D`: Rotate camera left/right around target

### Direct Movement
- `Page Up/Down`: Move camera up/down vertically
- `Left/Right`: Move camera left/right horizontally

### Additional Controls
- `Space`: Reset camera to default position and enable auto-rotation
- `Mouse Scroll`: Zoom in/out
- Any movement key automatically disables auto-rotation
- Auto-rotation resumes only when camera is reset to default position using Space

## Project Structure

- `window.h/cpp`: Window management and OpenGL context
- `camera.h/cpp`: Camera system and controls
- `shader.h/cpp`: Shader program management
- `cylinder.h/cpp`: Cylinder mesh generation
- `tree.h/cpp`: Tree generation algorithm
- `main.cpp`: Application entry point and rendering loop

## Troubleshooting

Common issues and solutions:

1. **Link errors**
   - Verify library paths in project properties
   - Check that GLFW and OpenGL32 libraries are properly linked

2. **Runtime errors**
   - Ensure all DLLs are in the executable directory
   - Verify that shader compilation succeeds

3. **Visual artifacts**
   - Make sure OpenGL version is 3.3 or higher
   - Check graphics driver updates
