cmake_minimum_required(VERSION 3.10)

project(piston VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(WIN32)
    add_compile_definitions(PISTON_OS_WINDOWS)
elseif(APPLE)
    add_compile_definitions(PISTON_OS_MAC)
elseif(UNIX)
    add_compile_definitions(PISTON_OS_LINUX)
endif()

add_subdirectory(library)
add_subdirectory(cli)