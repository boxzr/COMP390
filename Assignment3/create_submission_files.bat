@echo off
echo Creating submission files for Assignment 3...

echo Creating Program 1 submission file...
copy Program1\main.cpp COMP390_Assignment3_Program1_BoxerJonah.cpp

echo Creating Program 2 submission file...
copy Program2\main.cpp COMP390_Assignment3_Program2_BoxerJonah.cpp

echo Creating Program 3 submission file...
copy Program3\main.cpp COMP390_Assignment3_Program3_BoxerJonah.cpp

echo Done! Created the following files:
dir COMP390_Assignment3_*.cpp

echo.
echo Press any key to exit...
pause > nul 