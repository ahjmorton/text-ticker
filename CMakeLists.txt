cmake_minimum_required(VERSION 2.6)

project(text-ticker C)
enable_testing()

SET(CMAKE_C_FLAGS_PROFILING "-pg" CACHE STRING "Flags for profiling builds")
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(src)
add_subdirectory(test)
