cmake_minimum_required(VERSION 3.10)

project(ex1 VERSION 1.0)

file(GLOB ex1_SOURCES
    "*.cpp"
)

# Generate a shared library
add_library(${PROJECT_NAME} SHARED ${ex1_SOURCES} $<TARGET_OBJECTS:library>)

add_definitions(${library_DEFINITIONS})
add_dependencies(${PROJECT_NAME} library)
include_directories(${PROJECT_SOURCE_DIR}/include ${library_INCLUDE_DIRS})