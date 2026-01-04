# C Kata Collection

This project contains various coding katas converted from Python to C.

## Prerequisites

This project uses [cmocka](https://cmocka.org/) as the unit testing framework. You need to install it before building and running tests.

The Makefile automatically detects cmocka using pkg-config (Linux), Homebrew (macOS), or falls back to standard paths.

### Installing cmocka

**macOS (Homebrew):**

```bash
brew install cmocka
```

**Ubuntu/Debian:**

```bash
sudo apt-get install libcmocka-dev
```

**Fedora/RHEL:**

```bash
sudo dnf install libcmocka-devel
```

**From source:**

```bash
git clone https://git.cryptomilk.org/projects/cmocka.git
cd cmocka
mkdir build
cd build
cmake ..
make
sudo make install
```

## Structure

Each kata is in its own directory under `src/` with:

- `kata.c` - Implementation
- `kata.h` - Header file
- `test_kata.c` - Test file

## Building

To build all katas:

```bash
make
```

To build a specific kata:

```bash
make <kata_name>
# Example: make 1-fizz_buzz
```

To run tests for a specific kata:

```bash
make test_<kata_name>
# Example: make test_1-fizz_buzz
```

To run all tests:

```bash
make test
```

To clean build artifacts:

```bash
make clean
```

## Katas

1. 1-fizz_buzz - FizzBuzz kata
2. 2-leap_year - Leap year calculator
3. 3-fibonacci - Fibonacci sequence generator
4. 4-stack_kata - Stack data structure implementation
5. 5-roman_numerals - Roman numeral converter
6. 6-prime_factors - Prime factorization
7. 7-tic_tac_toe - Tic Tac Toe game
8. 8-yahtzee - Yahtzee scoring system
9. 9-mars_rover - Mars Rover navigation
10. 10-tennis - Tennis scoring system
11. 11-gilded_rose - Gilded Rose refactoring kata
12. 12-raid - Guild Raid kata
13. 14-smelly_tic_tac_toe - Code smells refactoring kata
14. 15-character_copier - Character copier with test doubles
15. 16-esa_mars_rover - ESA Mars Rover kata
16. 17-social_network - Social network implementation
17. 18-london_tic_tac_toe - London-style TDD Tic Tac Toe
