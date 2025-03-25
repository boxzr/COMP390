@echo off
echo Testing OpenGL functionality...

REM Check if Visual Studio is installed
if not exist "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" (
    echo ERROR: Visual Studio 2022 not found.
    echo Please enter the path to your Visual Studio installation directory:
    set /p VS_PATH=
) else (
    set VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community
)

REM Check if GLUT is installed
if not exist "C:\glut-3.7.6-bin\glut.h" (
    echo ERROR: GLUT not found at C:\glut-3.7.6-bin
    echo Please enter the path to your GLUT installation directory:
    set /p GLUT_PATH=
) else (
    set GLUT_PATH=C:\glut-3.7.6-bin
)

REM Create GL directory
if not exist "GL" mkdir GL

REM Copy GLUT header
echo Copying GLUT header file...
copy "%GLUT_PATH%\glut.h" "GL\"

REM Copy GLUT DLL
echo Copying GLUT DLL file...
copy "%GLUT_PATH%\glut32.dll" "."

REM Initialize Visual Studio environment
echo Setting up Visual Studio environment...
call "%VS_PATH%\Common7\Tools\VsDevCmd.bat"

REM Compile test program
echo Compiling test_gl.cpp...
cl test_gl.cpp /I. /link "%GLUT_PATH%\glut32.lib" opengl32.lib glu32.lib

REM Run test program if compilation succeeded
if exist test_gl.exe (
    echo Compilation successful!
    echo.
    echo Running OpenGL test program...
    echo If a window with a red triangle appears, OpenGL is working.
    echo.
    test_gl.exe
) else (
    echo ERROR: Compilation failed.
)

pause 