cmake_minimum_required(VERSION 3.25...4.2)
project(c-kata LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add compiler warnings
add_compile_options(-Wall -Wextra)

# Enable testing
enable_testing()

# Suppress warnings from FetchContent dependencies
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_WARN_DEPRECATED OFF)

# Fetch and build cmocka
include(FetchContent)
FetchContent_Declare(
    cmocka
    GIT_REPOSITORY https://git.cryptomilk.org/projects/cmocka.git
    GIT_TAG        cmocka-1.1.5
    GIT_SHALLOW    1
    TLS_VERIFY     false
)
set(WITH_STATIC_LIB ON CACHE BOOL "CMocka: Build with a static library" FORCE)
set(WITH_EXAMPLES OFF CACHE BOOL "CMocka: Build examples" FORCE)
set(UNIT_TESTING OFF CACHE BOOL "CMocka: Build with unit testing" FORCE)
set(PICKY_DEVELOPER OFF CACHE BOOL "CMocka: Build with picky developer flags" FORCE)
set(WITH_CMOCKERY_SUPPORT OFF CACHE BOOL "CMocka: Build with cmockery support" FORCE)
FetchContent_MakeAvailable(cmocka)

# List of all katas
set(KATAS
    1-fizz_buzz
    2-leap_year
    3-fibonacci
    4-stack_kata
    5-roman_numerals
    6-prime_factors
    7-tic_tac_toe
    8-yahtzee
    9-mars_rover
    10-tennis
    11-gilded_rose
    12-raid
    14-smelly_tic_tac_toe
    15-character_copier
    16-esa_mars_rover
    17-social_network
    18-london_tic_tac_toe
)

# Build each kata
foreach(KATA ${KATAS})
    set(KATA_DIR ${CMAKE_SOURCE_DIR}/src/${KATA})

    if(EXISTS ${KATA_DIR}/kata.c AND EXISTS ${KATA_DIR}/test_kata.c)
        # Create test executable for this kata
        add_executable(test_${KATA}
            ${KATA_DIR}/kata.c
            ${KATA_DIR}/test_kata.c
        )

        target_include_directories(test_${KATA} PRIVATE
            ${KATA_DIR}
        )

        target_link_libraries(test_${KATA}
            cmocka-static
        )

        # Add to CTest
        add_test(NAME ${KATA} COMMAND test_${KATA})
    endif()
endforeach()

# Custom target to run all tests
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS ${KATAS}
    COMMENT "Running all kata tests..."
)
