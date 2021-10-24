cmake_minimum_required(VERSION 3.10)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# set the project name
project(TFLite_Test VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(lib)