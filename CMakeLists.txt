# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name matches directory: Branch-Bound-Flowshop
project(Branch-Bound-Flowshop CXX)

# Set C++ standard to 17 (for modern features like structured bindings)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directory for header files
include_directories(include)

# Source files for the main executable
set(SOURCES
    src/main.cpp
    src/pfsp_instance.cpp
    src/ivm.cpp
    src/lower_bound.cpp
    src/utils.cpp
    src/bnb.cpp
)

# Add the main executable with a lowercase, hyphen-free name
add_executable(branch_bound_flowshop ${SOURCES})

