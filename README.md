# C Kata Collection

A collection of 17 coding katas in C, demonstrating test-driven development with CMake and cmocka.

## Prerequisites

- **CMake** 3.25 or newer
- **C11 compatible compiler** (GCC, Clang, MSVC)
- **Git** (for cloning the repository)

The project uses [cmocka](https://cmocka.org/) as the unit testing framework, which is **automatically downloaded and built** by CMake using FetchContent. No manual installation required!

## Quick Start

```bash
# Clone the repository
git clone https://github.com/pedromsantos/c-kata.git
cd c-kata

# Build all katas
make build

# Run all tests
make test
```

## Project Structure

```txt
c-kata/
├── CMakeLists.txt       # Main CMake configuration
├── Makefile             # Convenience wrapper around CMake
├── README.md
└── src/
    ├── 1-fizz_buzz/
    │   ├── kata.c       # Implementation
    │   ├── kata.h       # Header file
    │   ├── test_kata.c  # Test suite
    │   └── README.md    # Kata description
    ├── 2-leap_year/
    └── ...              # 17 katas total
```

## Building and Testing

The project uses **CMake** as the build system with a **Makefile** wrapper for convenience.

### Using Make (Recommended)

```bash
# Build all katas
make build

# Run all tests
make test

# Clean build artifacts
make clean

# Show all available targets
make help
```

### Using CMake Directly

```bash
# Configure and build
cmake -B build
cmake --build build

# Run tests
cd build
ctest --output-on-failure

# Or run specific test
./test_1-fizz_buzz
```

## Katas

This collection includes 17 coding katas of varying difficulty:

1. **1-fizz_buzz** - Classic FizzBuzz kata
2. **2-leap_year** - Leap year calculator
3. **3-fibonacci** - Fibonacci sequence generator
4. **4-stack_kata** - Stack data structure implementation
5. **5-roman_numerals** - Roman numeral converter
6. **6-prime_factors** - Prime factorization
7. **7-tic_tac_toe** - Tic Tac Toe game logic
8. **8-yahtzee** - Yahtzee scoring system
9. **9-mars_rover** - Mars Rover navigation
10. **10-tennis** - Tennis scoring system (Emily Bache's Tennis Refactoring Kata)
11. **11-gilded_rose** - Gilded Rose refactoring kata
12. **12-raid** - Guild Raid kata
13. **14-smelly_tic_tac_toe** - Code smells refactoring kata
14. **15-character_copier** - Character copier with test doubles
15. **16-esa_mars_rover** - ESA Mars Rover kata
16. **17-social_network** - Social network implementation
17. **18-london_tic_tac_toe** - London-style TDD Tic Tac Toe

Each kata directory contains a `README.md` with specific instructions and requirements.

## Features

- ✅ **Zero manual dependency installation** - CMake FetchContent handles cmocka automatically
- ✅ **Modern CMake practices** - Clean, maintainable build configuration
- ✅ **C11 standard** - Modern C language features
- ✅ **Comprehensive test coverage** - Each kata has a complete test suite
- ✅ **Cross-platform** - Works on Linux, macOS, and Windows
- ✅ **CTest integration** - Standard test runner with detailed output

## Development

### Compiler Flags

All projects are compiled with:

- `-std=c11` - C11 standard
- `-Wall -Wextra` - Enable all warnings

### CMake Configuration

The CMakeLists.txt automatically:

- Downloads and builds cmocka v1.1.5
- Creates test executables for all 17 katas
- Registers tests with CTest
- Generates compile_commands.json for IDE support

### Adding a New Kata

1. Create a new directory under `src/` (e.g., `src/19-new_kata/`)
2. Add `kata.c`, `kata.h`, and `test_kata.c`
3. CMake will automatically detect and build it

## License

This is an educational project for practicing TDD in C.
