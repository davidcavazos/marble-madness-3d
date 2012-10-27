cmake_minimum_required(VERSION 2.6)
project(marble-madness-3d)

# Compiler flags
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Weffc++")
#     set(CMAKE_EXE_LINKER_FLAGS "-lSDL")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})

include_directories(include)
add_subdirectory(src)
