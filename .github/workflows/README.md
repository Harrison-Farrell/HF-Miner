# GitHub Actions Workflows

This directory contains CI/CD workflows for the HF-Miner project.

## Available Workflows

### 1. CI Build and Test (`ci.yml`)
**Status Badge:** Comprehensive multi-platform testing
- **Trigger:** Push to main/develop/feature branches, PR to main/develop
- **Platforms:** Linux (Ubuntu), Windows, macOS
- **Build Types:** Debug and Release
- **Features:**
  - Multi-OS matrix testing
  - SHA256 unit tests
  - Block unit tests
  - Full test suite execution
  - Code coverage reporting (Linux only)
  - Coverage upload to Codecov

### 2. Quick Test (`test.yml`)
**Status Badge:** Fast test feedback
- **Trigger:** Push to main/develop, PR to main/develop
- **Platform:** Linux (Ubuntu) only
- **Build Type:** Release
- **Features:**
  - Quick feedback on test failures
  - Verbose test output
  - Fastest execution time

### 3. Multi-Platform Build (`build.yml`)
**Status Badge:** Build verification
- **Trigger:** Push to main/develop/feature branches, PR to main/develop
- **Platforms:** Linux (Ubuntu), Windows, macOS
- **Build Types:** Debug and Release
- **Features:**
  - Compilation verification on all platforms
  - No test execution (for speed)
  - Matrix strategy for all combinations

### 4. Code Quality (`quality.yml`)
**Status Badge:** Code quality and style checks
- **Trigger:** Push to main/develop, PR to main/develop
- **Platform:** Linux (Ubuntu) only
- **Build Type:** Debug
- **Features:**
  - Cppcheck static analysis
  - Clang-format code style verification
  - Unit test execution
  - Identifies potential issues early

## Recommended Branch Protection Rules

For production branches (`main`), consider enabling:
- ✅ Require CI Build and Test workflow to pass
- ✅ Require Quick Test workflow to pass
- ✅ Require status checks to pass before merging
- ✅ Dismiss stale PR approvals when new commits are pushed

## Workflow Details

### Dependencies Installed per Platform

**Ubuntu:**
- build-essential (gcc, g++, make)
- cmake
- ninja-build
- lcov (coverage reporting)
- cppcheck (code analysis)
- clang-format (code formatting)

**Windows:**
- cmake (via chocolatey)
- ninja-build (via chocolatey)
- MSVC compiler (pre-installed on windows-latest)

**macOS:**
- cmake (via brew)
- ninja (via brew)
- Apple Clang compiler (pre-installed)

### Build Configuration

- **CMake Version:** 3.30.0+
- **C++ Standard:** C++23
- **Generator:** Ninja (cross-platform)
- **Build Types:** Debug, Release

### Test Execution

Tests are executed using CTest with:
- Verbose output on failures
- Filtering by test name (SHA256, Block)
- Exit code validation

## Local Development

To replicate CI builds locally:

```bash
# Configure
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -G Ninja

# Build
cmake --build build --config Release

# Run tests
cd build
ctest --build-config Release --output-on-failure
```

## Coverage

Code coverage reports are generated on Linux only and uploaded to Codecov:
- Coverage includes all tests
- System headers and test code are excluded
- Coverage badge available via Codecov

## Troubleshooting

If workflows fail:

1. **Check platform-specific issues:**
   - Windows: Ensure Ninja installation completes
   - macOS: Verify Homebrew installation
   - Linux: Ensure apt-get cache is updated

2. **Build failures:**
   - Check CMake version compatibility
   - Verify all dependencies are installed
   - Review compiler output for detailed errors

3. **Test failures:**
   - Run `ctest --output-on-failure` locally
   - Check test logs in workflow run
   - Verify test data and fixtures

## Adding New Workflows

To add new workflows:
1. Create a `.yml` file in `.github/workflows/`
2. Define triggers and jobs
3. Test locally if possible
4. Commit and push to verify
