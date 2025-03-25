Write-Host "Setting up Visual Studio environment..."

# Set up Visual Studio environment
$VSPath = "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
$VSCmd = "`"$VSPath`" && "

Write-Host "Compiling Program 1 - Recursive Reflection..."

# Compile the program with Visual C++
$CompileCmd = "cl.exe /EHsc /W4 /Zi /Od /I. /Fe:program1.exe main.cpp /link opengl32.lib glu32.lib glut32.lib"
$FullCmd = "$VSCmd $CompileCmd"

# Execute the command
$process = Start-Process -FilePath "cmd.exe" -ArgumentList "/c", $FullCmd -Wait -PassThru -NoNewWindow

if ($process.ExitCode -ne 0) {
    Write-Host "Compilation failed."
    Read-Host "Press Enter to exit"
    exit $process.ExitCode
}

Write-Host "Compilation successful."
Write-Host "Running Program 1 - Recursive Reflection..."

# Run the program
Start-Process -FilePath ".\program1.exe" -NoNewWindow -Wait

Write-Host "Program execution completed."
Read-Host "Press Enter to exit" 