@echo off
echo opening vs developer prompt...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

echo making GL dir if needed...
if not exist "GL" mkdir GL

echo copying glut.h to GL dir...
copy "C:\glut-3.7.6-bin\glut.h" "GL\"

echo copying dll file...
copy "C:\glut-3.7.6-bin\glut32.dll" .

echo compiling program 1...
cl COMP390_Assignment1_Program1_BoxerJonah.cpp /I. /EHsc /Fe:Program1.exe /link "C:\glut-3.7.6-bin\glut32.lib" opengl32.lib glu32.lib
echo.

echo compiling program 2...
cl COMP390_Assignment1_Program2_BoxerJonah.cpp /I. /EHsc /Fe:Program2.exe /link "C:\glut-3.7.6-bin\glut32.lib" opengl32.lib glu32.lib
echo.

echo compiling program 3...
cl COMP390_Assignment1_Program3_BoxerJonah.cpp /I. /EHsc /Fe:Program3.exe /link "C:\glut-3.7.6-bin\glut32.lib" opengl32.lib glu32.lib
echo.

echo done compiling! 
echo.
echo here are the programs:
dir *.exe
echo.
echo press any key to run program 1...
pause > nul
start Program1.exe
echo.
echo press any key to run program 2...
pause > nul
start Program2.exe
echo.
echo press any key to run program 3...
pause > nul
start Program3.exe
echo.
echo all programs running now
echo.
pause 