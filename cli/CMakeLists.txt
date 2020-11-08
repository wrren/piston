cmake_minimum_required(VERSION 3.10)

project(cli)

file(GLOB_RECURSE CPP_FILES src/*.cpp)

add_definitions(${library_DEFINITIONS})

include_directories(${library_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${CPP_FILES})

target_link_libraries(${PROJECT_NAME} library)