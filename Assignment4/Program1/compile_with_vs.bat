@echo off
echo Setting up Visual Studio environment...

:: Set up Visual Studio environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

echo Compiling Program 1 - Recursive Reflection...

:: Compile the program with Visual C++
cl.exe /EHsc /W4 /Zi /Od /I. /D_CRT_SECURE_NO_DEPRECATE /DGLUT_DISABLE_ATEXIT_HACK /Fe:program1.exe main.cpp /link opengl32.lib glu32.lib glut32.lib

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