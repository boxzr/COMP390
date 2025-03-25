@echo off
echo Fixing Program 3 compilation...

REM Setup environment
set GLUT_PATH=C:\glut-3.7.6-bin
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

REM Create GL directory
if not exist "GL" mkdir GL
copy "%GLUT_PATH%\glut.h" "GL\" /Y

REM Navigate to Program3
cd Program3
copy "%GLUT_PATH%\glut32.dll" "." /Y

REM Generate texture if needed
if not exist "grass.bmp" (
    echo Creating grass texture...
    if exist "generate_texture.cpp" (
        cl generate_texture.cpp /nologo
        if exist generate_texture.exe (
            generate_texture.exe
        )
    )
)

REM Compile with different flags
echo Compiling Program 3 with different flags...
cl main.cpp /I.. /D_CRT_SECURE_NO_WARNINGS /wd4996 /D_ALLOW_KEYWORD_MACROS /Dnoreturn= /nologo /link "%GLUT_PATH%\glut32.lib" opengl32.lib glu32.lib

if exist main.exe (
    echo Compilation successful!
    echo.
    echo Program 3 Controls:
    echo - Press T to toggle texturing on/off
    echo - Use WASD or arrow keys to rotate view
    echo - Press ESC to exit
    echo.
    echo Running Program 3...
    main.exe
) else (
    echo ERROR: Compilation failed.
)

cd ..
pause 