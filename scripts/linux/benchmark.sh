#!/bin/bash

# benchmark.sh - Build with benchmarks enabled and run benchmark executables
# This script compiles the project with ENABLE_BENCHMARKS=ON and executes
# the benchmark targets for performance analysis.

set -e  # Exit on any error

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PROJECT_DIR="$SCRIPT_DIR/../../"
BUILD_TYPE="${BUILD_TYPE:-Debug}"
CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE:-Debug}"
NINJA_AVAILABLE=false

# List of benchmark targets to run (modify as needed)
declare -a BENCHMARK_TARGETS=("benchmark_sha256")

# Navigate to project directory
pushd ${PROJECT_DIR} > /dev/null

# Check for required tools
echo -e "${YELLOW}Checking prerequisites...${NC}"

if ! command -v cmake &> /dev/null; then
    echo -e "${RED}Error: CMake is not installed${NC}"
    exit 1
fi

if command -v ninja &> /dev/null; then
    NINJA_AVAILABLE=true
    GENERATOR="Ninja"
    echo -e "${GREEN}✓ Ninja found${NC}"
else
    GENERATOR="Unix Makefiles"
    echo -e "${YELLOW}⚠ Ninja not found, using Unix Makefiles${NC}"
fi

echo -e "${GREEN}✓ CMake found${NC}"

# Configure CMake with benchmarks enabled
echo -e "\n${YELLOW}Configuring CMake with ENABLE_BENCHMARKS=ON...${NC}"
cmake -S . -B "build" \
    -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" \
    -DENABLE_BENCHMARKS=ON \
    -DENABLE_UNIT_TESTING=ON \
    -G "$GENERATOR"

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ CMake configuration successful${NC}"
else
    echo -e "${RED}✗ CMake configuration failed${NC}"
    popd > /dev/null
    exit 1
fi

# Build the project
echo -e "\n${YELLOW}Building project with benchmarks...${NC}"
cmake --build "build" --config "$CMAKE_BUILD_TYPE"

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Build successful${NC}"
else
    echo -e "${RED}✗ Build failed${NC}"
    popd > /dev/null
    exit 1
fi

# Run benchmark executables
echo -e "\n${YELLOW}Running benchmark executables...${NC}"

BENCHMARK_DIR="build/benchmark"

if [ ! -d "$BENCHMARK_DIR" ]; then
    echo -e "${RED}Error: Benchmark directory not found at $BENCHMARK_DIR${NC}"
    popd > /dev/null
    exit 1
fi

for TARGET in "${BENCHMARK_TARGETS[@]}"; do
    BENCHMARK_PATH="$BENCHMARK_DIR/$TARGET"
    
    if [ ! -f "$BENCHMARK_PATH" ]; then
        echo -e "${YELLOW}⚠ Benchmark executable not found: $TARGET${NC}"
        continue
    fi

    echo -e "\n${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${YELLOW}Running benchmark: $TARGET${NC}"
    echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    
    if "$BENCHMARK_PATH"; then
        echo -e "${GREEN}✓ Benchmark '$TARGET' completed successfully${NC}"
    else
        echo -e "${RED}✗ Benchmark '$TARGET' failed${NC}"
    fi
done

popd > /dev/null
echo -e "\n${GREEN}✓ Benchmark execution completed${NC}"
