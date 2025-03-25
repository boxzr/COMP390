@echo off
echo Running Program 3 (Texture Mapping)...

REM Setup environment
set GLUT_PATH=C:\glut-3.7.6-bin
set VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community

REM Initialize Visual Studio environment
call "%VS_PATH%\Common7\Tools\VsDevCmd.bat"

REM Copy GLUT files
if not exist "GL" mkdir GL
copy "%GLUT_PATH%\glut.h" "GL\" /Y

REM Compile and run Program 3
cd Program3
copy "%GLUT_PATH%\glut32.dll" "." /Y

REM Generate texture if needed
if not exist "grass.bmp" (
    echo Generating texture...
    if exist "generate_texture.cpp" (
        cl generate_texture.cpp
        if exist generate_texture.exe (
            generate_texture.exe
        )
    ) else (
        echo WARNING: Texture generator not found, program may not display correctly.
    )
)

REM Compile the main program
cl main.cpp /I.. /link "%GLUT_PATH%\glut32.lib" opengl32.lib glu32.lib

if exist main.exe (
    echo.
    echo Program 3 Controls:
    echo - Press T to toggle texturing on/off
    echo - Use WASD or arrow keys to rotate view
    echo - Press ESC to exit
    echo.
    echo Running Program 3...
    main.exe
) else (
    echo ERROR: Compilation failed.
)

cd ..
pause 