COMP390 Assignment 3: Lighting and Surface Textures
====================================================

This assignment contains three OpenGL programs demonstrating lighting, material properties,
and texture mapping.

How to Run:
-----------

1. Run the batch file "run_in_vs_prompt.bat" to compile and execute all three programs.
   This will:
   - Set up the necessary OpenGL environment
   - Compile all three programs
   - Run each program one after another

For Program 3 (Textured Scene):
-------------------------------
Before running Program 3, you should generate a texture:

1. Navigate to the Program3 directory
2. Run "generate_texture.bat" to create a grass.bmp texture file
3. The texture will be used when Program3 runs

Program Descriptions:
--------------------

Program 1 - Lighting Elements and Surface Materials
* This program demonstrates OpenGL lighting features with a sphere.
* Three light sources illuminate the sphere from different angles.
* Right-click to access menus for selecting light colors and materials.
* Controls:
  - A/D: Rotate the sphere
  - ESC: Exit the program

Program 2 - Specular Lighting Calculation
* This program shows custom specular lighting calculations.
* The viewer is positioned at a distance to show specular highlights.
* Controls:
  - L: Toggle between OpenGL lighting and custom calculations
  - Arrow keys or WASD: Rotate the view
  - Number pad 1,2,4,6,8,9: Move the light source
  - ESC: Exit the program

Program 3 - Loaded Texture
* This program extends Assignment 2's scene with a textured ground.
* A procedural grass texture is applied to the ground.
* Houses are arranged in a circular pattern.
* Controls:
  - W: Toggle wireframe mode
  - ESC: Exit the program

Submission Files:
---------------
Run "create_submission_files.bat" to generate the properly named submission files.

The resulting files will be:
- COMP390_Assignment3_Program1_BoxerJonah.cpp
- COMP390_Assignment3_Program2_BoxerJonah.cpp
- COMP390_Assignment3_Program3_BoxerJonah.cpp 