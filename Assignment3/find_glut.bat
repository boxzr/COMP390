@echo off
echo Searching for GLUT files on your system...
echo.

set FOUND=0

if exist "C:\glut-3.7.6-bin\glut.h" (
    echo [FOUND] GLUT at C:\glut-3.7.6-bin
    echo       - glut.h: C:\glut-3.7.6-bin\glut.h
    echo       - glut32.dll: C:\glut-3.7.6-bin\glut32.dll
    echo       - glut32.lib: C:\glut-3.7.6-bin\glut32.lib
    set FOUND=1
)

if exist "C:\Program Files\Common Files\GLUT\glut.h" (
    echo [FOUND] GLUT at C:\Program Files\Common Files\GLUT
    echo       - glut.h: C:\Program Files\Common Files\GLUT\glut.h
    echo       - glut32.dll: C:\Program Files\Common Files\GLUT\glut32.dll
    echo       - glut32.lib: C:\Program Files\Common Files\GLUT\glut32.lib
    set FOUND=1
)

if exist "C:\Program Files (x86)\Common Files\GLUT\glut.h" (
    echo [FOUND] GLUT at C:\Program Files (x86)\Common Files\GLUT
    echo       - glut.h: C:\Program Files (x86)\Common Files\GLUT\glut.h
    echo       - glut32.dll: C:\Program Files (x86)\Common Files\GLUT\glut32.dll
    echo       - glut32.lib: C:\Program Files (x86)\Common Files\GLUT\glut32.lib
    set FOUND=1
)

if "%FOUND%"=="0" (
    echo [NOT FOUND] GLUT files were not found in standard locations.
    echo.
    echo If you have GLUT installed, please locate the glut.h, glut32.dll, and glut32.lib files.
    echo.
    echo You can download GLUT from: https://www.opengl.org/resources/libraries/glut/
    echo Extract the files and place them in C:\glut-3.7.6-bin
)

echo.
echo Searching for Visual Studio...
echo.

set VS_FOUND=0

if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" (
    echo [FOUND] Visual Studio 2022 Community at C:\Program Files\Microsoft Visual Studio\2022\Community
    set VS_FOUND=1
)

if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat" (
    echo [FOUND] Visual Studio 2022 Professional at C:\Program Files\Microsoft Visual Studio\2022\Professional
    set VS_FOUND=1
)

if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat" (
    echo [FOUND] Visual Studio 2022 Enterprise at C:\Program Files\Microsoft Visual Studio\2022\Enterprise
    set VS_FOUND=1
)

if "%VS_FOUND%"=="0" (
    echo [NOT FOUND] Visual Studio 2022 was not found in standard locations.
    echo Please make sure Visual Studio is installed with C++ development tools.
)

echo.
echo Press any key to exit...
pause > nul 