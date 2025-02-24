#------------------------------------------------------------------------------

# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)
project(michaeldello-cplusplus)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#------------------------------------------------------------------------------

# Generate debug symbols
#set(CMAKE_BUILD_TYPE Debug)

# Use -g for debug symbols
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

#------------------------------------------------------------------------------

# Define source files
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(TEST_DIR ${PROJECT_SOURCE_DIR}/tests)

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

#------------------------------------------------------------------------------

# Main executable
add_executable(run_cpp_examples ${SOURCES})

#------------------------------------------------------------------------------

# Enable Unit Testing using Catch2
enable_testing()

# Fetch Catch2
include(FetchContent)
FetchContent_Declare(
    catch2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.4.0.zip
)
FetchContent_MakeAvailable(catch2)

#------------------------------------------------------------------------------

file(GLOB_RECURSE TEST_SOURCES ${TEST_DIR}/*.cpp)

# Remove main() from unit test sources so unit testing doesn't enter a forever 
# loop
list(REMOVE_ITEM SOURCES ${SOURCE_DIR}/main.cpp)

foreach(TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE) # Extract test name
    add_executable(${TEST_NAME} ${TEST_SRC} ${SOURCES})
    target_link_libraries(${TEST_NAME} PRIVATE Catch2::Catch2WithMain)
    target_include_directories(${TEST_NAME} PRIVATE ${INCLUDE_DIR})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()