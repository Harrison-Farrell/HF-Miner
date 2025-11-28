# HF-Miner

A hobbyist attempt to mine me a bitcoin

## Build Status

![CI Build and Test](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/ci.yml/badge.svg)
![Quick Test](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/test.yml/badge.svg)
![Multi-Platform Build](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/build.yml/badge.svg)
![Code Quality](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/quality.yml/badge.svg)

## Building

### Prerequisites

- CMake 3.30.0 or later
- C++23 compatible compiler (GCC 13+, Clang 16+, or MSVC 2022)
- Ninja (recommended) or Make

### Build Instructions

```bash
# Configure
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -G Ninja

# Build
cmake --build build --config Release

# Run Tests
cd build
ctest --build-config Release --output-on-failure
```

## Testing

The project includes comprehensive unit tests for SHA256 and Block components:

```bash
cd build

# Run all tests
ctest --output-on-failure

# Run specific test suites
ctest -R SHA256 --output-on-failure
ctest -R Block --output-on-failure
```

## CI/CD

GitHub Actions workflows are configured for:
- Multi-platform builds (Linux, Windows, macOS)
- Automated unit test execution
- Code quality checks (cppcheck, clang-format)
- Code coverage reporting

See [.github/workflows/README.md](.github/workflows/README.md) for details.