cmake_minimum_required(VERSION 3.10)

project(homeworld VERSION 1.0)

file(GLOB homeworld_SOURCES
    "*.cpp"
)

# Generate a shared library
add_library(${PROJECT_NAME} SHARED ${homeworld_SOURCES} $<TARGET_OBJECTS:library>)

add_definitions(${library_DEFINITIONS})
add_dependencies(${PROJECT_NAME} library)
include_directories(${PROJECT_SOURCE_DIR}/include ${library_INCLUDE_DIRS})