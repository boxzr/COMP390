@echo off
echo Running Assignment 3 Programs Compilation and Execution
echo ======================================================

REM Setup environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
echo Visual Studio Developer Command Prompt activated.

REM Define GLUT path
SET GLUT_PATH=C:\glut-3.7.6-bin
echo Using GLUT from: %GLUT_PATH%

REM Create GL directory if it doesn't exist
if not exist "GL" (
    echo Creating GL directory...
    mkdir GL
)

REM Copy glut.h to GL directory
echo Copying glut.h to GL directory...
copy "%GLUT_PATH%\glut.h" "GL\" /Y

REM ============== PROGRAM 2 ==============
echo.
echo Compiling Program 2: Specular Lighting Calculation
echo -------------------------------------------------

REM Copy required files for Program 2
echo Copying glut32.dll to Program2 directory...
copy "%GLUT_PATH%\glut32.dll" "Program2\" /Y

REM Compile Program 2
cd Program2
echo Compiling main.cpp with debug information...
cl main.cpp /I.. /EHsc /DDEBUG /link "%GLUT_PATH%\glut32.lib" opengl32.lib glu32.lib
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Compilation of Program 2 failed.
    cd ..
    pause
    exit /b 1
)
cd ..

REM ============== PROGRAM 3 ==============
echo.
echo Compiling Program 3: Texture Mapping
echo -----------------------------------

REM Create texture for Program 3
echo Generating texture for Program 3...
cd Program3
if exist "generate_texture.cpp" (
    echo Compiling texture generator...
    cl generate_texture.cpp /EHsc
    if %ERRORLEVEL% EQU 0 (
        echo Running texture generator...
        generate_texture.exe
    ) else (
        echo Warning: Failed to compile texture generator. Using existing texture if available.
    )
) else (
    echo Texture generator not found, creating a simple texture...
    echo Writing simple BMP texture generator...
    
    echo #include ^<stdio.h^> > simple_texture_gen.cpp
    echo #include ^<stdlib.h^> >> simple_texture_gen.cpp
    echo #pragma pack(push, 1) >> simple_texture_gen.cpp
    echo typedef struct { >> simple_texture_gen.cpp
    echo     unsigned short bfType; >> simple_texture_gen.cpp
    echo     unsigned int bfSize; >> simple_texture_gen.cpp
    echo     unsigned short bfReserved1; >> simple_texture_gen.cpp
    echo     unsigned short bfReserved2; >> simple_texture_gen.cpp
    echo     unsigned int bfOffBits; >> simple_texture_gen.cpp
    echo } BITMAPFILEHEADER; >> simple_texture_gen.cpp
    echo typedef struct { >> simple_texture_gen.cpp
    echo     unsigned int biSize; >> simple_texture_gen.cpp
    echo     int biWidth; >> simple_texture_gen.cpp
    echo     int biHeight; >> simple_texture_gen.cpp
    echo     unsigned short biPlanes; >> simple_texture_gen.cpp
    echo     unsigned short biBitCount; >> simple_texture_gen.cpp
    echo     unsigned int biCompression; >> simple_texture_gen.cpp
    echo     unsigned int biSizeImage; >> simple_texture_gen.cpp
    echo     int biXPelsPerMeter; >> simple_texture_gen.cpp
    echo     int biYPelsPerMeter; >> simple_texture_gen.cpp
    echo     unsigned int biClrUsed; >> simple_texture_gen.cpp
    echo     unsigned int biClrImportant; >> simple_texture_gen.cpp
    echo } BITMAPINFOHEADER; >> simple_texture_gen.cpp
    echo #pragma pack(pop) >> simple_texture_gen.cpp
    echo int main() { >> simple_texture_gen.cpp
    echo     const int width = 256; >> simple_texture_gen.cpp
    echo     const int height = 256; >> simple_texture_gen.cpp
    echo     const int dataSize = width * height * 3; >> simple_texture_gen.cpp
    echo     const int fileSize = 54 + dataSize; >> simple_texture_gen.cpp
    echo     unsigned char* img = (unsigned char*)malloc(dataSize); >> simple_texture_gen.cpp
    echo     FILE* f = fopen("grass.bmp", "wb"); >> simple_texture_gen.cpp
    echo     if (!f || !img) { printf("Error creating texture\n"); return 1; } >> simple_texture_gen.cpp
    echo     for(int y = 0; y < height; y++) { >> simple_texture_gen.cpp
    echo         for(int x = 0; x < width; x++) { >> simple_texture_gen.cpp
    echo             int idx = (y * width + x) * 3; >> simple_texture_gen.cpp
    echo             int isEvenX = (x / 32) %% 2; >> simple_texture_gen.cpp
    echo             int isEvenY = (y / 32) %% 2; >> simple_texture_gen.cpp
    echo             if ((isEvenX && isEvenY) || (!isEvenX && !isEvenY)) { >> simple_texture_gen.cpp
    echo                 img[idx+0] = 34;   // B >> simple_texture_gen.cpp
    echo                 img[idx+1] = 139;  // G >> simple_texture_gen.cpp
    echo                 img[idx+2] = 34;   // R >> simple_texture_gen.cpp
    echo             } else { >> simple_texture_gen.cpp
    echo                 img[idx+0] = 0;    // B >> simple_texture_gen.cpp
    echo                 img[idx+1] = 252;  // G >> simple_texture_gen.cpp
    echo                 img[idx+2] = 124;  // R >> simple_texture_gen.cpp
    echo             } >> simple_texture_gen.cpp
    echo         } >> simple_texture_gen.cpp
    echo     } >> simple_texture_gen.cpp
    echo     BITMAPFILEHEADER bfh = {0}; >> simple_texture_gen.cpp
    echo     bfh.bfType = 0x4D42; >> simple_texture_gen.cpp
    echo     bfh.bfSize = fileSize; >> simple_texture_gen.cpp
    echo     bfh.bfOffBits = 54; >> simple_texture_gen.cpp
    echo     BITMAPINFOHEADER bih = {0}; >> simple_texture_gen.cpp
    echo     bih.biSize = 40; >> simple_texture_gen.cpp
    echo     bih.biWidth = width; >> simple_texture_gen.cpp
    echo     bih.biHeight = height; >> simple_texture_gen.cpp
    echo     bih.biPlanes = 1; >> simple_texture_gen.cpp
    echo     bih.biBitCount = 24; >> simple_texture_gen.cpp
    echo     fwrite(&bfh.bfType, 2, 1, f); >> simple_texture_gen.cpp
    echo     fwrite(&bfh.bfSize, 4, 1, f); >> simple_texture_gen.cpp
    echo     fwrite(&bfh.bfReserved1, 2, 1, f); >> simple_texture_gen.cpp
    echo     fwrite(&bfh.bfReserved2, 2, 1, f); >> simple_texture_gen.cpp
    echo     fwrite(&bfh.bfOffBits, 4, 1, f); >> simple_texture_gen.cpp
    echo     fwrite(&bih, 40, 1, f); >> simple_texture_gen.cpp
    echo     fwrite(img, dataSize, 1, f); >> simple_texture_gen.cpp
    echo     free(img); >> simple_texture_gen.cpp
    echo     fclose(f); >> simple_texture_gen.cpp
    echo     printf("Created grass.bmp texture (256x256)\n"); >> simple_texture_gen.cpp
    echo     return 0; >> simple_texture_gen.cpp
    echo } >> simple_texture_gen.cpp
    
    cl simple_texture_gen.cpp
    simple_texture_gen.exe
)

REM Check if texture was created
if not exist "grass.bmp" (
    echo Warning: Texture file not found. Program 3 may not display correctly.
)

REM Copy required files for Program 3
echo Copying glut32.dll to Program3 directory...
copy "%GLUT_PATH%\glut32.dll" ".\" /Y

REM Compile Program 3
echo Compiling main.cpp with debug information...
cl main.cpp /I.. /EHsc /DDEBUG /link "%GLUT_PATH%\glut32.lib" opengl32.lib glu32.lib
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Compilation of Program 3 failed.
    cd ..
    pause
    exit /b 1
)
cd ..

REM ============== RUN PROGRAMS ==============
echo.
echo ======================================================
echo Compilation complete for Assignment 3 programs!
echo.
echo You can now run:
echo   1. Program 2 - Specular Lighting Calculation
echo   2. Program 3 - Texture Mapping
echo.

choice /C 123 /M "Enter your choice (1-2) or 3 to exit: "
if %ERRORLEVEL% EQU 1 (
    cd Program2
    echo Running Program 2: Specular Lighting Calculation...
    echo Controls: W,A,S,D or arrow keys to rotate, L to toggle between custom and OpenGL lighting, ESC to exit
    main.exe
    cd ..
) else if %ERRORLEVEL% EQU 2 (
    cd Program3
    echo Running Program 3: Texture Mapping...
    echo Controls: W,A,S,D or arrow keys to rotate, T to toggle texturing, ESC to exit
    main.exe
    cd ..
) else (
    echo Exiting without running any programs.
)

echo.
echo Program execution completed.
pause 