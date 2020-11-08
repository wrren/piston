cmake_minimum_required(VERSION 3.10)

# Define Project
project(library)

# Find all C++ Source Files
file(GLOB_RECURSE UNIVERSAL_CPP_FILES src/core/*.cpp src/process/*.cpp)

if(WIN32)
    add_compile_definitions(PISTON_OS_WINDOWS)
elseif(APPLE)
    add_compile_definitions(PISTON_OS_MAC)
elseif(UNIX)
    add_compile_definitions(PISTON_OS_LINUX)
endif()

if(WIN32)
file(GLOB_RECURSE PLATFORM_CPP_FILES src/platform/windows/*.cpp)
elseif(APPLE)
file(GLOB_RECURSE PLATFORM_CPP_FILES src/platform/mac/*.cpp)
elseif(UNIX)
file(GLOB_RECURSE PLATFORM_CPP_FILES src/platform/linux/*.cpp)
endif()

# Generate a shared library
add_library(${PROJECT_NAME} SHARED ${UNIVERSAL_CPP_FILES} ${PLATFORM_CPP_FILES})

# Add the library include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Set the exported include directories for this library
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)