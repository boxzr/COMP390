@echo off
echo Debugging Program 2...
echo.

echo Opening VS Developer Command Prompt...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

echo Creating GL directory if needed...
if not exist "GL" mkdir GL

echo Setting up GLUT...
set GLUT_PATH=C:\glut-3.7.6-bin
echo Using GLUT at %GLUT_PATH%
copy "%GLUT_PATH%\glut.h" "GL\"
copy "%GLUT_PATH%\glut32.dll" Program2\

echo Compiling Program 2 with extra debug info...
cd Program2
cl main.cpp /I.. /EHsc /Zi /DDEBUG /link "%GLUT_PATH%\glut32.lib" opengl32.lib glu32.lib

echo Contents of Program2 directory:
dir

echo GLUT32.DLL file info:
if exist glut32.dll echo GLUT32.DLL exists in the current directory
if not exist glut32.dll echo ERROR: GLUT32.DLL is missing!

echo.
echo Program 2 instructions:
echo - Press L to toggle lighting mode
echo - Use arrow keys to rotate the view
echo - Use numpad to move light source
echo - Press ESC to exit
echo.

echo Press any key to run Program 2 with debug output...
pause > nul
echo Running Program 2...
Program2.exe
echo.

cd ..
echo Debug session complete.
echo Press any key to exit...
pause > nul 