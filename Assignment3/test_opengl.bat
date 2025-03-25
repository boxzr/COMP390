@echo off
echo Opening VS Developer Command Prompt...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

echo Creating GL directory if needed...
if not exist "GL" mkdir GL

REM Try different possible GLUT locations
set GLUT_PATH=
if exist "C:\glut-3.7.6-bin" set GLUT_PATH=C:\glut-3.7.6-bin
if exist "C:\Program Files\Common Files\GLUT" set GLUT_PATH=C:\Program Files\Common Files\GLUT
if exist "C:\Program Files (x86)\Common Files\GLUT" set GLUT_PATH=C:\Program Files (x86)\Common Files\GLUT

if "%GLUT_PATH%"=="" (
    echo ERROR: Could not find GLUT files.
    echo Please enter the path to your GLUT installation:
    set /p GLUT_PATH=
)

echo Found GLUT at %GLUT_PATH%
copy "%GLUT_PATH%\glut.h" "GL\"
copy "%GLUT_PATH%\glut32.dll" "."

echo Compiling test program...
cl test_opengl.cpp /I. /EHsc /Fe:test_opengl.exe /link "%GLUT_PATH%\glut32.lib" opengl32.lib glu32.lib
if exist test_opengl.exe (
    echo Test program compiled successfully!
    echo Press any key to run the test program...
    pause > nul
    test_opengl.exe
) else (
    echo Failed to compile test program!
)

echo Press any key to exit...
pause > nul 