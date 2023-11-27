enable_testing()

include(FetchContent)
include(GoogleTest)

if(NOT TARGET gtest_main)
  FetchContent_Declare(
    googletest URL https://github.com/google/googletest/archive/main.zip)
  FetchContent_MakeAvailable(googletest)
endif()

add_executable(ecpp_endian_ut src/00_byteswap.cpp src/01_ntoh_hton.cpp)
target_include_directories(ecpp_endian_ut PUBLIC include)
target_link_libraries(ecpp_endian_ut ecpp::endian gtest_main)
target_compile_options(ecpp_endian_ut PUBLIC -pthread)
target_link_options(ecpp_endian_ut PUBLIC -pthread)

target_compile_options(
  ecpp_endian_ut
  PUBLIC -Wall
         -Wextra
         -pedantic
         -Wconversion
         -Wimplicit
         -Winit-self
         -Wcast-qual
         -Wcast-align
         -Wpacked
         -Wstrict-overflow=5
         -Wstrict-aliasing
         -fstrict-aliasing
         -Wredundant-decls
         -Wmissing-declarations
         -Wdisabled-optimization
         -Wold-style-cast
         -Wno-gnu-zero-variadic-macro-arguments)

gtest_discover_tests(ecpp_endian_ut)

add_custom_command(
  TARGET ecpp_endian_ut
  COMMENT "Run tests"
  POST_BUILD
  COMMAND ctest ARGS --output-on-failure
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
