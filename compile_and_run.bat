@echo off
title Number System Converter - Activity No. 1
echo ============================================================
echo   Number System Converter - Activity No. 1
echo   Compiling and Running with C++ (Standard C++11)...
echo ============================================================

REM Source files
set SOURCES=main.cpp src\ui.cpp src\conversion.cpp src\arithmetic.cpp src\bcd.cpp src\display.cpp

REM Check if Dev-Cpp MinGW is in default path
set COMPILER="C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\g++.exe"
if exist %COMPILER% (
    %COMPILER% -std=c++11 -Wall -Wextra -Iinclude %SOURCES% -o converter.exe
    goto RUN
)

REM Fallback to standard PATH g++
where g++ >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    g++ -std=c++11 -Wall -Wextra -Iinclude %SOURCES% -o converter.exe
    goto RUN
)

REM Fallback to pre-built executable if available
if exist converter.exe (
    echo Compiler not found in PATH, running existing converter.exe...
    goto RUN
)

echo [!] Error: C++ compiler (g++) not found.
echo Please open the project in Dev-C++ or Code::Blocks and compile manually.
echo Add all files in src\ to your project alongside main.cpp.
pause
exit /b 1

:RUN
if exist converter.exe (
    cls
    converter.exe
) else (
    echo [!] Compilation failed. Check the error messages above.
    pause
)
