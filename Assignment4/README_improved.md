# Assignment 4: Enhanced Ray Tracing Implementation

This assignment implements improved versions of the two ray tracing programs:

1. **Program 1: Recursive Reflection** - A recursive ray tracing algorithm that simulates reflections on surfaces.
2. **Program 2: Shadows** - A ray tracing algorithm that implements shadow effects.

## Improved Implementation

Both programs have been optimized to more closely follow the recursive ray tracing algorithm pseudo-code. The implementations now explicitly use the following structure:

```
Routine: Recursive Ray Tracer
Return value: Colour Contribution
Parameters: Incident ray, cumulative recursive ray tracer calls

If termination condition is met (maximum recursive calls or too far away)
    Return "no contribution"
Else
    Use the Incident Ray to find the nearest intersection
    Cast a ray from the intersection to the light sources

    For each light source
        Find if there is any intersection point in between
        If intersection exists
            Calculate lighting intensity with shadow
        Else
            Calculate lighting intensity without shadow
        Endif

        Calculate local color (the surface color of the local object)
        
        If the object at the intersection point is reflective
            Calculate reflective ray
            Call Recursive Ray Tracer to get reflective color
        Endif

        Color per light source = 
            (Local color * local color contribution percentage) +
            (Reflective color * reflective color contribution percentage)
    EndFor

    Add all colors per light source to get color at the intersection point
    Return color at the intersection point
```

## Key Improvements

1. **Explicit Termination Conditions**:
   - Maximum recursion depth checking
   - No intersection detection (too far away)

2. **Shadow Calculation**:
   - Explicit shadow rays cast towards light sources
   - Shadow check before adding diffuse and specular components

3. **Local vs. Reflected Color**:
   - Clear separation between local color calculation and reflection
   - Proper handling of contribution percentages

4. **Performance Optimization**:
   - Early termination for performance
   - Careful management of recursion depth

## How to Compile and Run

### Program 1: Recursive Reflection

```
cd Assignment4\Program1
compile_simple.bat
```

### Program 2: Shadows

```
cd Assignment4\Program2
compile_simple.bat
```

## Notes

These simplified implementations use the Windows API and OpenGL for display purposes. They avoid the GLUT dependencies which were causing compilation issues.

The core ray tracing algorithms have been improved while maintaining the same visual output as the original implementations. 