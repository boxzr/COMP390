@echo off
echo Opening VS Developer Command Prompt...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

echo Creating GL directory if needed...
if not exist "GL" mkdir GL

REM Try different possible GLUT locations
set GLUT_FOUND=0
if exist "C:\glut-3.7.6-bin\glut.h" (
    echo Found GLUT at C:\glut-3.7.6-bin
    copy "C:\glut-3.7.6-bin\glut.h" "GL\"
    copy "C:\glut-3.7.6-bin\glut32.dll" Program2\
    set GLUT_LIB_PATH=C:\glut-3.7.6-bin\glut32.lib
    set GLUT_FOUND=1
) else if exist "C:\Program Files\Common Files\GLUT\glut.h" (
    echo Found GLUT at C:\Program Files\Common Files\GLUT
    copy "C:\Program Files\Common Files\GLUT\glut.h" "GL\"
    copy "C:\Program Files\Common Files\GLUT\glut32.dll" Program2\
    set GLUT_LIB_PATH="C:\Program Files\Common Files\GLUT\glut32.lib"
    set GLUT_FOUND=1
) else if exist "C:\Program Files (x86)\Common Files\GLUT\glut.h" (
    echo Found GLUT at C:\Program Files (x86)\Common Files\GLUT
    copy "C:\Program Files (x86)\Common Files\GLUT\glut.h" "GL\"
    copy "C:\Program Files (x86)\Common Files\GLUT\glut32.dll" Program2\
    set GLUT_LIB_PATH="C:\Program Files (x86)\Common Files\GLUT\glut32.lib"
    set GLUT_FOUND=1
)

if "%GLUT_FOUND%"=="0" (
    echo ERROR: Could not find GLUT files.
    echo Please download GLUT from https://www.opengl.org/resources/libraries/glut/
    echo and extract to C:\glut-3.7.6-bin or specify the path in this batch file.
    goto :end
)

echo Compiling Program 2...
cd Program2
cl main.cpp /I.. /EHsc /Fe:Program2.exe /link %GLUT_LIB_PATH% opengl32.lib glu32.lib
if exist Program2.exe (
    echo Program 2 compiled successfully!
    echo Press any key to run Program 2...
    pause > nul
    Program2.exe
) else (
    echo Failed to compile Program 2!
)
cd ..

:end
echo Press any key to exit...
pause > nul 