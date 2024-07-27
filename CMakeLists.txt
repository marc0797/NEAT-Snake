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
file(GLOB_RECURSE NEAT_SOURCES "src/NEAT/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")
file(GLOB_RECURSE NEAT_HEADERS "include/NEAT/*.hpp")

add_executable(NEAT_Snake ${SOURCES} ${HEADERS})
# Add executables for each file in the test folder
file(GLOB TEST_SOURCES "tests/*.cpp")
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE} ${NEAT_SOURCES} ${NEAT_HEADERS})
endforeach()

# Link SFML libraries
target_link_libraries(NEAT_Snake sfml-system sfml-window sfml-graphics)
