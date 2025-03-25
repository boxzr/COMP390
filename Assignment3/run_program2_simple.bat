@echo off
echo Running Program 2 (Specular Lighting)...

REM Setup environment
set GLUT_PATH=C:\glut-3.7.6-bin
set VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community

REM Initialize Visual Studio environment
call "%VS_PATH%\Common7\Tools\VsDevCmd.bat"

REM Copy GLUT files
if not exist "GL" mkdir GL
copy "%GLUT_PATH%\glut.h" "GL\" /Y

REM Compile and run Program 2
cd Program2
copy "%GLUT_PATH%\glut32.dll" "." /Y
cl main.cpp /I.. /link "%GLUT_PATH%\glut32.lib" opengl32.lib glu32.lib

if exist main.exe (
    echo.
    echo Program 2 Controls:
    echo - Press L to toggle between OpenGL lighting and custom lighting
    echo - Use WASD or arrow keys to rotate view
    echo - Use numpad 1,2,4,6,8,9 to move light source
    echo - Press ESC to exit
    echo.
    echo Running Program 2...
    main.exe
) else (
    echo ERROR: Compilation failed.
)

cd ..
pause 