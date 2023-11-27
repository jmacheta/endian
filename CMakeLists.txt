cmake_minimum_required(VERSION 3.21)

project(
  ecpp_endian
  LANGUAGES CXX
  VERSION 0.9.0)

option(CONFIG_ECPP_ENDIAN_BUILD_TESTS "Forces building test cases" OFF)
option(CONFIG_ECPP_ENDIAN_BUILD_EXAMPLES "Forces building examples" OFF)

add_library(ecpp_endian INTERFACE)
target_include_directories(ecpp_endian INTERFACE include)
target_compile_features(ecpp_endian INTERFACE cxx_std_20)

add_library(ecpp::endian ALIAS ecpp_endian)

if(PROJECT_IS_TOP_LEVEL OR CONFIG_ECPP_ENDIAN_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(PROJECT_IS_TOP_LEVEL OR CONFIG_ECPP_ENDIAN_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
