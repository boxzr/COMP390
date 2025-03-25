@echo off
echo Creating GL directory if it doesn't exist...
if not exist "GL" mkdir GL
echo Copying glut.h to the GL directory...
copy "C:\glut-3.7.6-bin\glut.h" "GL\"

echo Compiling Program 1...
cl COMP390_Assignment1_Program1_BoxerJonah.cpp /I. /EHsc /Fe:Program1.exe /link "C:\glut-3.7.6-bin\glut32.lib" opengl32.lib glu32.lib
if %ERRORLEVEL% NEQ 0 (
    echo Compilation of Program 1 failed!
    pause
    exit /b
)
echo.
echo Compiling Program 2...
cl COMP390_Assignment1_Program2_BoxerJonah.cpp /I. /EHsc /Fe:Program2.exe /link "C:\glut-3.7.6-bin\glut32.lib" opengl32.lib glu32.lib
if %ERRORLEVEL% NEQ 0 (
    echo Compilation of Program 2 failed!
    pause
    exit /b
)
echo.
echo Compiling Program 3...
cl COMP390_Assignment1_Program3_BoxerJonah.cpp /I. /EHsc /Fe:Program3.exe /link "C:\glut-3.7.6-bin\glut32.lib" opengl32.lib glu32.lib
if %ERRORLEVEL% NEQ 0 (
    echo Compilation of Program 3 failed!
    pause
    exit /b
)

echo Copying glut32.dll to the project directory...
copy "C:\glut-3.7.6-bin\glut32.dll" .

echo.
echo All programs compiled successfully.
echo.
dir *.exe
echo.
echo To run Program 1: Program1.exe
echo To run Program 2: Program2.exe
echo To run Program 3: Program3.exe
echo.
pause 