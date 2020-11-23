cmake_minimum_required(VERSION 3.10)

# Define Project
project(library)

# Find all C++ Source Files
file(GLOB_RECURSE 
    UNIVERSAL_SOURCES 
    src/piston/core/*.cpp 
    include/piston/core/*.h 
    src/piston/code/*.cpp 
    include/piston/code/*.h
    src/piston/file/*.cpp 
    include/piston/file/*.h
    src/piston/process/*.cpp 
    include/piston/process/*.h
)

# Set up platform-specific sources
if(WIN32)
    file(GLOB_RECURSE PLATFORM_SOURCES src/piston/platform/windows/*.cpp include/piston/platform/*.h)
elseif(APPLE)
    file(GLOB_RECURSE PLATFORM_SOURCES src/piston/platform/mac/*.cpp include/piston/platform/*.h)
elseif(UNIX)
    file(GLOB_RECURSE PLATFORM_SOURCES src/piston/platform/linux/*.cpp include/piston/platform/*.h)
endif()

# Set the combined source list for the project
set(SOURCE_LIST ${UNIVERSAL_SOURCES} ${PLATFORM_SOURCES})

# Generate a shared library
add_library(${PROJECT_NAME} STATIC ${SOURCE_LIST})

# Generate a source file tree for IDEs like Visual Studio
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})

# Add the library include directory
include_directories(${PROJECT_SOURCE_DIR}/include ${WIL_INCLUDE_DIRS})

# Set the exported include directories for this library
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)