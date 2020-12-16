cmake_minimum_required(VERSION 3.10)

project(tests VERSION 1.0)

file(GLOB_RECURSE tests_SOURCES
    "*.cpp"
)

# Generate an executable
add_executable(${PROJECT_NAME} ${tests_SOURCES})

add_dependencies(${PROJECT_NAME} library)
add_definitions(${library_DEFINITIONS})
target_link_libraries(${PROJECT_NAME} library)
include_directories(${PROJECT_SOURCE_DIR}/include ${library_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/../../deps/catch2/single_include)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME} -o report.xml -r junit)