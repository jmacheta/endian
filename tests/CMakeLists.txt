cmake_policy(SET CMP0135 NEW)

include(FetchContent)

FetchContent_Declare(
  googletest URL https://github.com/google/googletest/archive/main.zip FIND_PACKAGE_ARGS NAMES
                 GTest
)
FetchContent_MakeAvailable(googletest)

add_executable(ecpp_endian_ut byteswap.cpp ntoh_hton.cpp)
target_include_directories(ecpp_endian_ut PUBLIC include)
target_link_libraries(ecpp_endian_ut ecpp::endian gtest_main)
target_compile_options(ecpp_endian_ut PUBLIC -pthread)
target_link_options(ecpp_endian_ut PUBLIC -pthread)

target_compile_options(
  ecpp_endian_ut
  PRIVATE -Wall
          -Wextra
          -pedantic
          -Wconversion
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
)

include(GoogleTest)

gtest_discover_tests(ecpp_endian_ut)
