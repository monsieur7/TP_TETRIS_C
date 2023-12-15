# CMake minimum version required
cmake_minimum_required(VERSION 3.12)

# Project name
project(TETRIS_TEST_C)

# Set C++ standard
set(CMAKE_C_STANDARD 11)

# Add executable
add_executable(tetris_test_c main.c tetris.c)

# You can add more source files if needed
# add_executable(tetris_test_c main.c another_file.c)

# Specify additional include directories if needed
# target_include_directories(tetris_test_c PUBLIC include)

# Specify additional libraries if needed
# target_link_libraries(tetris_test_c PUBLIC some_library)