# HF-Miner

A hobbyist attempt to mine me a bitcoin

## Build Status

![CI Build and Test](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/ci.yml/badge.svg)
![Quick Test](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/test.yml/badge.svg)
![Multi-Platform Build](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/build.yml/badge.svg)
![Code Quality](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/quality.yml/badge.svg)
![Memory Check](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/memcheck.yml/badge.svg)

## Building

### Prerequisites

- **CMake** 3.30.0 or later
- **C++23 compatible compiler** (GCC 13+, Clang 16+, or MSVC 2022)
- **Ninja** (recommended) or Make

### Build Instructions

```bash
# Configure (unit tests enabled by default)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -G Ninja

# Build
cmake --build build --config Release

# Run Tests
cd build
ctest --build-config Release --output-on-failure
```

### Build Configuration Options

CMake options are manually controlled and default as follows:

| Option | Default | Purpose |
|--------|---------|---------|
| `ENABLE_UNIT_TESTING` | ON | Enable/disable test targets |
| `ENABLE_FORMAT` | OFF | Enable/disable automatic code formatting |
| `ENABLE_BENCHMARKS` | OFF | Enable/disable performance benchmark targets |
| `ENABLE_DOXYGEN` | OFF | Enable/disable API documentation generation |
| `ENABLE_CPPCHECK` | OFF | Enable/disable static code analysis integration |
| `ENABLE_COVERAGE` | OFF | Enable/disable code coverage analysis |
| `ENABLE_MEMCHECK` | OFF | Enable/disable memory leak detection (Unix/Linux only) |

### Platform-Specific Tools

| Tool | Platform | Purpose | CMake Option |
|------|----------|---------|--------------|
| `lcov` / `genhtml` | Linux | Code coverage reporting | ENABLE_COVERAGE |
| `llvm-cov` / `llvm-profdata` | macOS, Clang/Windows | Code coverage reporting | ENABLE_COVERAGE |
| `valgrind` | Linux | Memory checking backend | ENABLE_MEMCHECK |

Override defaults explicitly:

```bash
# Enable specific features
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DENABLE_BENCHMARKS=ON -G Ninja

# Disable specific features
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_UNIT_TESTING=OFF -G Ninja
```
## CI/CD

GitHub Actions workflows are configured for:
- **CI Build and Test** (`ci.yml`) - Multi-platform builds (Linux, Windows, macOS) with automated unit test execution and code coverage
- **Quick Test** (`test.yml`) - Fast feedback on unit tests on Linux
- **Multi-Platform Build** (`build.yml`) - Build verification across platforms without tests
- **Code Quality** (`quality.yml`) - Static analysis and code formatting checks
- **Memory Check** (`memcheck.yml`) - Valgrind-based memory leak detection on Linux

See [.github/workflows/README.md](.github/workflows/README.md) for details.
See [.github/workflows/README.md](.github/workflows/README.md) for details.