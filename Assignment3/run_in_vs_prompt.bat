@echo off
echo opening vs developer prompt...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

echo creating GL directory if needed...
if not exist "GL" mkdir GL

echo copying glut.h to GL dir...
copy "C:\glut-3.7.6-bin\glut.h" "GL\"

echo copying dll files...
copy "C:\glut-3.7.6-bin\glut32.dll" Program1\
copy "C:\glut-3.7.6-bin\glut32.dll" Program2\
copy "C:\glut-3.7.6-bin\glut32.dll" Program3\

echo compiling texture generator...
cd Program3
cl generate_texture.cpp /EHsc
if exist generate_texture.exe (
    echo running texture generator...
    generate_texture.exe
) else (
    echo texture generator compilation failed, using built-in texture.
)
cd ..
echo.

echo compiling program 1...
cd Program1
cl main.cpp /I.. /EHsc /Fe:Program1.exe /link "C:\glut-3.7.6-bin\glut32.lib" opengl32.lib glu32.lib
echo.

echo compiling program 2...
cd ..\Program2
cl main.cpp /I.. /EHsc /Fe:Program2.exe /link "C:\glut-3.7.6-bin\glut32.lib" opengl32.lib glu32.lib
echo.

echo compiling program 3...
cd ..\Program3
cl main.cpp /I.. /EHsc /Fe:Program3.exe /link "C:\glut-3.7.6-bin\glut32.lib" opengl32.lib glu32.lib
echo.
cd ..

echo done compiling!
echo.
echo here are the programs:
dir Program1\*.exe Program2\*.exe Program3\*.exe
echo.
echo press any key to run program 1...
pause > nul
cd Program1
Program1.exe
cd ..
echo.
echo press any key to run program 2...
pause > nul
cd Program2
Program2.exe
cd ..
echo.
echo press any key to run program 3...
pause > nul
cd Program3
Program3.exe
cd ..
echo.
echo all programs running!
echo.
pause 