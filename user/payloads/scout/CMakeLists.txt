cmake_minimum_required(VERSION 3.10)

project(scout VERSION 1.0)

file(GLOB scout_SOURCES
    "*.cpp"
)

# Generate a shared library
add_library(${PROJECT_NAME} SHARED ${scout_SOURCES} $<TARGET_OBJECTS:library>)

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_definitions(${library_DEFINITIONS})
add_dependencies(${PROJECT_NAME} library)
include_directories(${PROJECT_SOURCE_DIR}/include ${library_INCLUDE_DIRS})