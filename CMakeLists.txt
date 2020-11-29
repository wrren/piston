cmake_minimum_required(VERSION 3.10)

project(piston VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(WIN32)
    # Set the Windows preprocessor definition
    add_compile_definitions(PISTON_OS_WINDOWS UNICODE)

    # Set our preferred Windows SDK Version
    set (CMAKE_SYSTEM_VERSION 10.0.17763.0)
    
    # Include the WIL library, disable redundant build targets like packaging and testing
    set(FAST_BUILD ON CACHE BOOL "Enable WIL Fast Build" FORCE)
    set(WIL_BUILD_PACKAGING OFF CACHE BOOL "Disable WIL Build Packaging" FORCE)
    set(WIL_BUILD_TESTS OFF CACHE BOOL "Disable WIL Test Build" FORCE)
    set(WIL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/deps/wil/include")
    add_subdirectory(deps/wil)
elseif(APPLE)
    add_compile_definitions(PISTON_OS_MAC)
elseif(UNIX)
    add_compile_definitions(PISTON_OS_LINUX)
endif()

add_subdirectory(library)
add_subdirectory(cli)
add_subdirectory(payloads)