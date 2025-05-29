
@echo off
cd /d %~dp0
echo Building with CMake...
cmake -S . -B build
cmake --build build --config Debug
if %errorlevel% neq 0 (
    echo Build failed.
    exit /b %errorlevel%
)
echo Running executable...
cd build/Debug
GndG3.0.exe
pause
