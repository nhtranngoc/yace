cmake_minimum_required(VERSION 3.10)

# Include GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Test targets
add_executable(tests main.cpp chip8.cpp opcodes.cpp)
add_test(NAME tests COMMAND tests)

# Link library from src/chip8 + pthread
target_link_libraries(tests PUBLIC chip8_lib ${GTEST_LIBRARIES} pthread)
