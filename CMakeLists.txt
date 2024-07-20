cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(NEAT_Snake VERSION 0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the required libraries
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

# Specify the include directories
include_directories(include)

# Add the executable
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

add_executable(NEAT_Snake ${SOURCES} ${HEADERS})

# Link SFML libraries
target_link_libraries(NEAT_Snake sfml-system sfml-window sfml-graphics)
