# Assignment 4: 3D Graphics

This assignment focuses on implementing two ray tracing programs:

1. **Program 1: Recursive Reflection** - Implements a recursive ray tracing algorithm that simulates reflections on the floor surface.
2. **Program 2: Shadows** - Implements shadow effects by checking if intersection points can see the light source.

## Program 1: Recursive Reflection

This program creates a scene with colored walls and a reflective floor. The rays are recursively traced to calculate reflections on the floor surface with a maximum recursion depth of 5. Key features:

- A room with red, green, and blue walls
- Reflective floor surface
- Diffuse lighting calculations
- Recursive ray tracing for reflections

To compile and run:
```
cd Program1
compile_and_run.bat
```

Press ESC to exit the program.

## Program 2: Shadows

This program creates a scene with several colored spheres on a checkerboard floor. The algorithm checks if intersection points can see the light source to determine if they are in shadow. Key features:

- Multiple colored spheres
- Checkerboard floor pattern
- Shadow calculations
- Diffuse and specular lighting

To compile and run:
```
cd Program2
compile_and_run.bat
```

Press ESC to exit the program.

## Implementation Details

### Recursive Reflection (Program 1)
The `trace()` function is implemented recursively to handle reflections. When a ray hits the floor, a reflection ray is cast and the colors are mixed based on the reflection coefficient.

### Shadows (Program 2)
The `isInShadow()` function casts a ray from the intersection point toward the light source. If this ray intersects any object before reaching the light, the intersection point is considered to be in shadow.

## Requirements

- OpenGL/GLUT
- C++ compiler (e.g., G++)
- Windows environment (for the batch files)

## Notes

- The programs use the ray tracing approach to render the scenes pixel by pixel.
- Lighting calculations include ambient, diffuse, and specular components.
- The batch files are configured for Windows with specific paths to OpenGL libraries. 