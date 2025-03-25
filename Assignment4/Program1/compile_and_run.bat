@echo off
echo Compiling Program 1 - Recursive Reflection...

:: Set paths for OpenGL libraries
set INCLUDE_PATH=-I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um" -I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\shared"
set LIB_PATH=-L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x86"

:: Compile the program
g++ -std=c++11 main.cpp -o program1.exe %INCLUDE_PATH% %LIB_PATH% -lopengl32 -lglu32 -lfreeglut

if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    pause
    exit /b %ERRORLEVEL%
)

echo Compilation successful.
echo Running Program 1 - Recursive Reflection...

:: Run the program
program1.exe

echo Program execution completed.
pause 