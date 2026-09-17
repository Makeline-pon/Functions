@echo off
chcp 65001 >nul

where g++.exe >nul 2>nul
if errorlevel 1 (
    echo [ERROR] g++ not found.
    echo Please install MinGW-w64 and add its "bin" directory to system PATH.
    echo Download: https://winlibs.com
    pause
    exit /b 1
)

echo Compiling...
g++ -std=c++26 -O2 -Wall main.cpp Function.cpp -o Function.exe -static-libgcc -static-libstdc++

if errorlevel 1 (
    echo.
    echo [ERROR] Compilation failed.
    pause
    exit /b 1
)

echo.
echo Build successful! Running...
echo ========================================
echo.
.\Function.exe

echo.
pause
