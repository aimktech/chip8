cmake_minimum_required(VERSION 3.15)

message("")
message( STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR} )
message( STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR} )
message( STATUS "CMAKE_COMMAND: " ${CMAKE_COMMAND} )
message( STATUS "CMAKE_VERSION: " ${CMAKE_VERSION} )
message( STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER} )
message( STATUS "CMAKE_PROJECT_NAME: " ${CMAKE_PROJECT_NAME} )
message( STATUS "CMAKE_FINDCMAKE: " ${CMAKE_SOURCE_DIR}/CMAKE )

project("chip8")

# ------- Set Vars ------- #

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMAKE")

set(CHIP8_DEBUG OFF)

# locate the SDL2 library
find_package(SDL2 REQUIRED)
message("")
message( STATUS "FINDING SDL2" )
if (${SDL2_FOUND})
    message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    message( STATUS "SDL2_INCLUDE_DIR:" ${SDL2_INCLUDE_DIRS})
    message( STATUS "SDL2_LIBRARY: " ${SDL2_LIBRARIES})
else()
    message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    message( FATAL_ERROR "SDL2 NOT FOUND" )
endif()

if(CHIP8_DEBUG)
add_compile_definitions(CHIP8_DEBUG)
endif()

include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${SDL2_INCLUDE_DIRS}/..)

file(GLOB SOURCES src/*.cpp)

add_executable(chip8 ${SOURCES})

target_link_libraries(chip8 ${SDL2_LIBRARIES})
