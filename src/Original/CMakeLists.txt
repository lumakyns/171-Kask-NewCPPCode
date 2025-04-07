cmake_minimum_required(VERSION 3.10)
project(Checkers_cpp)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Needed for Clang-based tools

# Directories
include_directories("include")
file(GLOB SOURCE_FILES "src/*.cpp")

# Flags for compilation, if needed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Add executable with the name 'Checkers'
add_executable("Checkers" ${SOURCE_FILES})
