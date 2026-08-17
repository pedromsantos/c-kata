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
    3-stats_calculator
    4-anagrams
    5-fibonacci
    6-stack_kata
    7-roman_numerals
    8-prime_factors
    9-tic_tac_toe
    10-yahtzee
    11-mars_rover
    12-tennis
    13-gilded_rose
    14-raid
    16-smelly_tic_tac_toe
    17-character_copier
    18-esa_mars_rover
    19-social_network
    20-london_tic_tac_toe
    21-smelly_mars_rover
    22-smelly_shopping_cart
    23-shopping_cart
    24-katacombs
    26-smelly_yahtzee
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

# 22-smelly_shopping_cart is laid out across Domain/Application/Infrastructure
# (mirroring the hexagonal ts-kata source), so it needs its own executable
# instead of the single kata.c/test_kata.c pair the loop above expects.
set(SMELLY_SHOPPING_CART_DIR ${CMAKE_SOURCE_DIR}/src/22-smelly_shopping_cart)

if(EXISTS ${SMELLY_SHOPPING_CART_DIR}/test_kata.c)
    add_executable(test_22-smelly_shopping_cart
        ${SMELLY_SHOPPING_CART_DIR}/Domain/domain.c
        ${SMELLY_SHOPPING_CART_DIR}/Infrastructure/infrastructure.c
        ${SMELLY_SHOPPING_CART_DIR}/Application/application.c
        ${SMELLY_SHOPPING_CART_DIR}/test_kata.c
    )

    target_include_directories(test_22-smelly_shopping_cart PRIVATE
        ${SMELLY_SHOPPING_CART_DIR}
    )

    target_link_libraries(test_22-smelly_shopping_cart
        cmocka-static
    )

    add_test(NAME 22-smelly_shopping_cart COMMAND test_22-smelly_shopping_cart)
endif()

# Custom target to run all tests
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS ${KATAS}
    COMMENT "Running all kata tests..."
)
