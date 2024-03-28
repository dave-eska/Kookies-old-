cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("thirdparty/raylib")
add_subdirectory("thirdparty/jsoncpp")

set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)

#project
project(main1)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)

FetchContent_Declare(
box2d
URL https://github.com/erincatto/box2d/archive/refs/tags/v2.4.1.tar.gz
)
FetchContent_MakeAvailable(box2d)

# Define MY_SOURCES to be a list of all the source files for my project
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable(main1 "${MY_SOURCES}")

target_include_directories(main1 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(main1 PRIVATE raylib jsoncpp_static box2d)
