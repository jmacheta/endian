cmake_minimum_required(VERSION 3.25)

project(endian LANGUAGES CXX VERSION 1.0.2)

option(ENDIAN_BUILD_TESTS "Configure unit tests target" OFF)
option(ENDIAN_BUILD_EXAMPLES "Configure examples target" OFF)
option(ENDIAN_BUILD_INSTALL "Configure install target" ON)

add_library(endian INTERFACE)
target_include_directories(
  endian INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                   $<INSTALL_INTERFACE:include>
)
target_compile_features(endian INTERFACE cxx_std_20)

add_library(ecpp::endian ALIAS endian)

if (PROJECT_IS_TOP_LEVEL OR ENDIAN_BUILD_INSTALL)
  enable_testing()
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/install.cmake)
endif ()

if (PROJECT_IS_TOP_LEVEL OR ENDIAN_BUILD_EXAMPLES)
  add_subdirectory(examples EXCLUDE_FROM_ALL)
endif ()

if (PROJECT_IS_TOP_LEVEL OR ENDIAN_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests EXCLUDE_FROM_ALL)
endif ()
