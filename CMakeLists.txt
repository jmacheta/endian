cmake_minimum_required(VERSION 3.14)

project(ecpp_endian)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

add_library(ecpp_endian INTERFACE)
target_include_directories(ecpp_endian INTERFACE include)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  # This is a standalone build, so fetch GoogleTest and build unit tests
  add_subdirectory(tests)
endif()
