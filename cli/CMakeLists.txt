cmake_minimum_required(VERSION 3.10)

project(cli)

file(GLOB_RECURSE CPP_FILES src/*.cpp)

add_executable(${PROJECT_NAME} ${CPP_FILES} $<TARGET_OBJECTS:library>)

add_definitions(${library_DEFINITIONS})

if(WIN32)
add_dependencies(${PROJECT_NAME} WIL)
target_link_libraries(${PROJECT_NAME} WIL)
endif()

add_dependencies(${PROJECT_NAME} library)
target_link_libraries(${PROJECT_NAME} library)

include_directories(${PROJECT_SOURCE_DIR}/include ${library_INCLUDE_DIRS} ${WIL_INCLUDE_DIRS})

