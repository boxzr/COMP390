@echo off
echo Running Assignment 3 Programs
echo ===========================

REM Setup environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
echo Visual Studio Developer Command Prompt activated.

REM Define GLUT path
SET GLUT_PATH=C:\glut-3.7.6-bin
echo Using GLUT from: %GLUT_PATH%

REM ============== COMPILE AND RUN PROGRAM 2 ==============
echo.
echo Compiling Program 2: Specular Lighting Calculation...

cd Program2
copy "%GLUT_PATH%\glut32.dll" ".\" /Y > nul
cl main.cpp /I.. /EHsc /DDEBUG /link "%GLUT_PATH%\glut32.lib" opengl32.lib glu32.lib > nul

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile Program 2.
    cd ..
    pause
    exit /b 1
)

echo Running Program 2...
echo Controls: W,A,S,D or arrow keys to rotate, L to toggle between custom and OpenGL lighting, ESC to exit
start /wait main.exe
cd ..
echo.

REM ============== COMPILE AND RUN PROGRAM 3 ==============
echo.
echo Compiling Program 3: Texture Mapping...

cd Program3
copy "%GLUT_PATH%\glut32.dll" ".\" /Y > nul

REM Generate texture if needed
if not exist "grass.bmp" (
    echo Generating texture...
    if exist "generate_texture.cpp" (
        cl generate_texture.cpp /EHsc > nul
        generate_texture.exe
    ) else (
        echo Texture generator not found, skipping...
    )
)

cl main.cpp /I.. /EHsc /DDEBUG /link "%GLUT_PATH%\glut32.lib" opengl32.lib glu32.lib > nul

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile Program 3.
    cd ..
    pause
    exit /b 1
)

echo Running Program 3...
echo Controls: W,A,S,D or arrow keys to rotate, T to toggle texturing, ESC to exit
start /wait main.exe
cd ..

echo.
echo All programs completed!
pause 