# HF-Miner

A hobbyist attempt to mine me a bitcoin

## Build Status

![CI Build and Test](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/ci.yml/badge.svg)
![Quick Test](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/test.yml/badge.svg)
![Multi-Platform Build](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/build.yml/badge.svg)
![Code Quality](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/quality.yml/badge.svg)
![Memory Check](https://github.com/Harrison-Farrell/HF-Miner/actions/workflows/memcheck.yml/badge.svg)

## Development Container

This project includes a **Visual Studio Code Dev Container** configuration for consistent development environments across platforms.

### Using the Dev Container

1. **Install Prerequisites**:
   - [Docker](https://www.docker.com/)
   - [Visual Studio Code](https://code.visualstudio.com/)
   - [Dev Containers Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)

2. **Open in Container**:
   - Open the workspace folder in VS Code
   - Click the "Reopen in Container" button, or
   - Use Command Palette (`Ctrl+Shift+P`) → "Dev Containers: Reopen in Container"

3. **Features Included**:
   - C++23 compiler (GCC 13)
   - CMake 3.30+
   - Ninja build system
   - Git
   - Valgrind (for memory checking)
   - Google Benchmark and Test frameworks
   - Code formatting and analysis tools
   - VS Code extensions: C++ tools, Git Graph, Clang Format, Markdown

## Building

### Prerequisites (Local Development)

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

## Helper Scripts

Convenience scripts are provided for common development tasks:

| Script | Platform | Purpose | Command |
|--------|----------|---------|---------|
| `memcheck.sh` | Linux | Compile with memory checking and run Valgrind analysis | `./scripts/linux/memcheck.sh` |
| `benchmark.sh` | Linux | Compile with benchmarks and run performance tests | `./scripts/linux/benchmark.sh` |
| `benchmark.bat` | Windows | Compile with benchmarks and run performance tests | `.\scripts\windows\benchmark.bat` |

### Script Usage

**Linux environment variables**:
```bash
# Run benchmarks in Debug mode
CMAKE_BUILD_TYPE=Debug ./scripts/linux/benchmark.sh

# Run memcheck in Release mode
CMAKE_BUILD_TYPE=Release ./scripts/linux/memcheck.sh
```

**Windows**:
```batch
REM Runs with Debug configuration by default
.\scripts\windows\benchmark.bat
```

**Note**: The Windows `benchmark.bat` script uses Visual Studio 2022 by default; Ninja is detected and used if available.

## CI/CD

GitHub Actions workflows are configured for:
- **CI Build and Test** (`ci.yml`) - Multi-platform builds (Linux, Windows, macOS) with automated unit test execution and code coverage
- **Quick Test** (`test.yml`) - Fast feedback on unit tests on Linux
- **Multi-Platform Build** (`build.yml`) - Build verification across platforms without tests
- **Code Quality** (`quality.yml`) - Static analysis and code formatting checks
- **Memory Check** (`memcheck.yml`) - Valgrind-based memory leak detection on Linux

See [.github/workflows/README.md](.github/workflows/README.md) for details.
See [.github/workflows/README.md](.github/workflows/README.md) for details.