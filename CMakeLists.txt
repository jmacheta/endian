cmake_minimum_required(VERSION 3.25)

project(endian LANGUAGES CXX VERSION 1.0.5)

option(ENDIAN_TESTS "Configure unit tests target" ${PROJECT_IS_TOP_LEVEL})
option(ENDIAN_EXAMPLES "Configure examples target" ${PROJECT_IS_TOP_LEVEL})
option(ENDIAN_INSTALL "Configure install target" ON)

add_library(endian INTERFACE)
target_include_directories(
  endian INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                   $<INSTALL_INTERFACE:include>
)

target_compile_features(endian INTERFACE cxx_std_20)

add_library(ecpp::endian ALIAS endian)

if (ENDIAN_INSTALL)
  include(cmake/install.cmake)
endif ()

if (ENDIAN_EXAMPLES)
  add_subdirectory(examples EXCLUDE_FROM_ALL)
endif ()

if (ENDIAN_TESTS)
  include(CTest)
  add_subdirectory(tests EXCLUDE_FROM_ALL)
endif ()
