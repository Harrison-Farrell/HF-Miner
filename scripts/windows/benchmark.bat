@echo off
REM benchmark.bat - Build with benchmarks enabled and run benchmark executables
REM This script compiles the project with ENABLE_BENCHMARKS=ON and executes
REM the benchmark targets for performance analysis.

setlocal enabledelayedexpansion

REM Configuration
set "SCRIPT_DIR=%~dp0"
set "PROJECT_DIR=%SCRIPT_DIR%..\.."
set "BUILD_TYPE=Debug"
set "CMAKE_BUILD_TYPE=Debug"
set "NINJA_AVAILABLE=0"
set "GENERATOR=Visual Studio 17 2022"

REM Array of benchmark targets to run
set "BENCHMARK_TARGETS=benchmark_sha256"

REM Navigate to project directory
pushd "%PROJECT_DIR%"

REM Check for required tools
echo Checking prerequisites...

where cmake >nul 2>&1
if errorlevel 1 (
    echo Error: CMake is not installed
    popd
    exit /b 1
)

echo CMake found

where ninja >nul 2>&1
if errorlevel 1 (
    echo Ninja not found, using Visual Studio 2022
) else (
    set "NINJA_AVAILABLE=1"
    set "GENERATOR=Ninja"
    echo Ninja found
)

REM Configure CMake with benchmarks enabled
echo.
echo Configuring CMake with ENABLE_BENCHMARKS=ON...
cmake -S . -B "build" ^
    -DCMAKE_BUILD_TYPE="%CMAKE_BUILD_TYPE%" ^
    -DENABLE_BENCHMARKS=ON ^
    -DENABLE_UNIT_TESTING=ON ^
    -G "%GENERATOR%"

if errorlevel 1 (
    echo CMake configuration failed
    popd
    exit /b 1
)

echo CMake configuration successful

REM Build the project
echo.
echo Building project with benchmarks...
cmake --build "build" --config "%CMAKE_BUILD_TYPE%"

if errorlevel 1 (
    echo Build failed
    popd
    exit /b 1
)

echo Build successful

REM Run benchmark executables
echo.
echo Running benchmark executables...

set "BENCHMARK_DIR=build\benchmark"

if not exist "%BENCHMARK_DIR%" (
    echo Error: Benchmark directory not found at %BENCHMARK_DIR%
    popd
    exit /b 1
)

for %%T in (%BENCHMARK_TARGETS%) do (
    set "BENCHMARK_PATH=%BENCHMARK_DIR%\%%T.exe"
    
    if not exist "!BENCHMARK_PATH!" (
        echo Benchmark executable not found: %%T
    ) else (
        echo.
        echo ========================================================================
        echo Running benchmark: %%T
        echo ========================================================================
        
        "!BENCHMARK_PATH!"
        if errorlevel 1 (
            echo Benchmark '%%T' failed
        ) else (
            echo Benchmark '%%T' completed successfully
        )
    )
)

popd
echo.
echo Benchmark execution completed
