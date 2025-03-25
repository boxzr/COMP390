# COMP390 - Computer Graphics Assignment

This repository contains three OpenGL programs for the COMP390 assignment:

## Programs

1. **Program 1**: Anti-aliasing demonstration with two houses - one with standard lines and one with anti-aliased lines.
2. **Program 2**: 2D Graphics Transformation showing five houses arranged in a circle using rotation and translation.
3. **Program 3**: Coloring and Shading with a square that has color gradients between red, green, blue, and yellow vertices.

## How to Run

1. Make sure you have Visual Studio with C++ development tools installed
2. Double-click the `run_in_vs_prompt.bat` batch file to:
   - Set up the GLUT environment
   - Compile all three programs
   - Run each program

## Dependencies

- OpenGL
- GLUT (OpenGL Utility Toolkit)
- Visual Studio C++ compiler

## Prerequisites

- OpenGL
- GLUT (OpenGL Utility Toolkit)
- C++ compiler

## Building the Programs

### Using CMake (Recommended for Windows)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

This will create the executables in the build directory.

### Using Makefile (Linux/Mac)

```bash
make all
```

This will compile all three programs, creating executables in their respective directories.

## Running the Programs

### Program 1: Anti-aliasing

```bash
# From the build directory (CMake)
./Program1

# Or using the Makefile build
./Program1/program1
```

This program shows two houses - the top one drawn with regular lines and the bottom one with anti-aliased lines.

### Program 2: 2D Graphics Transformation

```bash
# From the build directory (CMake)
./Program2

# Or using the Makefile build
./Program2/program2
```

This program demonstrates 2D transformations with five houses arranged in a circle.

### Program 3: Coloring and Shading

```bash
# From the build directory (CMake)
./Program3

# Or using the Makefile build
./Program3/program3
```

This program shows a square with color interpolation between red, green, blue, and yellow vertices.

## Controls

There are no interactive controls for these programs. Close the window to exit each program. 