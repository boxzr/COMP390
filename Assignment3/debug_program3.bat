@echo off
echo Debugging Program 3...
echo.

echo Opening VS Developer Command Prompt...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

echo Creating GL directory if needed...
if not exist "GL" mkdir GL

echo Setting up GLUT...
set GLUT_PATH=C:\glut-3.7.6-bin
echo Using GLUT at %GLUT_PATH%
copy "%GLUT_PATH%\glut.h" "GL\"
copy "%GLUT_PATH%\glut32.dll" Program3\

echo Creating texture for Program 3...
cd Program3
if exist generate_texture.cpp (
    echo Compiling texture generator...
    cl generate_texture.cpp /EHsc
    if exist generate_texture.exe (
        echo Running texture generator...
        generate_texture.exe
    ) else (
        echo ERROR: Failed to compile texture generator!
    )
) else (
    echo WARNING: No texture generator found.
)

echo Texture file check:
if exist grass.bmp (
    echo grass.bmp exists
) else (
    echo WARNING: grass.bmp is missing!
)

echo Compiling Program 3 with extra debug info...
cl main.cpp /I.. /EHsc /Zi /DDEBUG /link "%GLUT_PATH%\glut32.lib" opengl32.lib glu32.lib

echo Contents of Program3 directory:
dir

echo GLUT32.DLL file info:
if exist glut32.dll echo GLUT32.DLL exists in the current directory
if not exist glut32.dll echo ERROR: GLUT32.DLL is missing!

echo.
echo Program 3 instructions:
echo - Press W to toggle wireframe mode
echo - Press ESC to exit
echo.

echo Press any key to run Program 3 with debug output...
pause > nul
echo Running Program 3...
Program3.exe
echo.

cd ..
echo Debug session complete.
echo Press any key to exit...
pause > nul 