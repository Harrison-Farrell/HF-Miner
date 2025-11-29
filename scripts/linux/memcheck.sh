#!/bin/bash

# memcheck.sh - Build with memcheck enabled and run memory checking
# This script compiles the project with ENABLE_MEMCHECK=ON and executes
# the memcheck targets for detecting memory leaks and errors.

set -e  # Exit on any error

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Configuration
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PROJECT_DIR="$SCRIPT_DIR/../../"
BUILD_TYPE="${BUILD_TYPE:-Debug}"
BUILD_DIR="build"
CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE:-Debug}"
NINJA_AVAILABLE=false

# List of memcheck targets to run (modify as needed)
declare -a MEMCHECK_TARGETS=("memcheck-test_sha256" "memcheck-test_block")

# Navigate to project directory
pushd ${PROJECT_DIR} > /dev/null

# Check if running on Unix/Linux
if [[ ! "$OSTYPE" == "linux-gnu"* ]]; then
    echo -e "${RED}Error: Memcheck is only supported on Unix/Linux systems${NC}"
    exit 1
fi

# Check for required tools
echo -e "${YELLOW}Checking prerequisites...${NC}"

if ! command -v cmake &> /dev/null; then
    echo -e "${RED}Error: CMake is not installed${NC}"
    exit 1
fi

if ! command -v valgrind &> /dev/null; then
    echo -e "${RED}Error: Valgrind is not installed${NC}"
    echo "Install with: sudo apt-get install valgrind"
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
echo -e "${GREEN}✓ Valgrind found${NC}"

# Configure CMake with memcheck enabled
echo -e "\n${YELLOW}Configuring CMake with ENABLE_MEMCHECK=ON...${NC}"
cmake -S . -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" \
    -DENABLE_MEMCHECK=ON \
    -DENABLE_UNIT_TESTING=ON \
    -G "$GENERATOR"

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ CMake configuration successful${NC}"
else
    echo -e "${RED}✗ CMake configuration failed${NC}"
    exit 1
fi

# Build the project
echo -e "\n${YELLOW}Building project...${NC}"
cmake --build "$BUILD_DIR" --config "$CMAKE_BUILD_TYPE"

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Build successful${NC}"
else
    echo -e "${RED}✗ Build failed${NC}"
    exit 1
fi

# Find and run all memcheck targets
echo -e "\n${YELLOW}Running memcheck targets...${NC}"

# Change to build directory
cd "$BUILD_DIR"

# List of memcheck targets (modify as needed)
MEMCHECK_TARGETS=("memcheck-test_sha256" "memcheck-test_block")   

if [ -z "$MEMCHECK_TARGETS" ]; then
    echo -e "${YELLOW}No memcheck targets found. Building and running unit tests first...${NC}"
    ctest --build-config "$CMAKE_BUILD_TYPE" --output-on-failure
    cd ..
    echo -e "${GREEN}Unit tests completed${NC}"
    exit 0
fi

for TARGET in "${MEMCHECK_TARGETS[@]}"; do
    echo -e "\n${YELLOW}Running memcheck target: $TARGET...${NC}"
    if cmake --build . --target "$TARGET" --config "$CMAKE_BUILD_TYPE"; then
        echo -e "${GREEN}✓ Memcheck target '$TARGET' completed successfully${NC}"
    else
        echo -e "${RED}✗ Memcheck target '$TARGET' failed${NC}"
    fi
done

# reset path
popd