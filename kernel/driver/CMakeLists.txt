cmake_minimum_required(VERSION 3.10)

project(driver)

file(GLOB_RECURSE CPP_FILES src/*.cpp)

wdk_add_driver(${PROJECT_NAME} ${CPP_FILES})

if(WIN32)
add_dependencies(${PROJECT_NAME} WIL)
target_link_libraries(${PROJECT_NAME} WIL)
endif()

