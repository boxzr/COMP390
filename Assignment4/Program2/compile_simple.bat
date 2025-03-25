@echo off
echo Setting up Visual Studio environment...

:: Set up Visual Studio environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

echo Compiling Program 2 - Shadows (Simplified Version)...

:: Compile the program with Visual C++
cl.exe /EHsc /W4 /Zi /Od /Fe:program2_simple.exe simple_version.cpp /link opengl32.lib gdi32.lib user32.lib

if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    pause
    exit /b %ERRORLEVEL%
)

echo Compilation successful.
echo Running Program 2 - Shadows (Simplified Version)...

:: Run the program
program2_simple.exe

echo Program execution completed.
pause 