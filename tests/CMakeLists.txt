include(FetchContent)

FetchContent_Declare(
  googletest URL https://github.com/google/googletest/archive/main.zip FIND_PACKAGE_ARGS NAMES
                 GTest
)
FetchContent_MakeAvailable(googletest)

add_executable(endian_ut byteswap.cpp ntoh_hton.cpp)
target_include_directories(endian_ut PUBLIC include)
target_link_libraries(endian_ut ecpp::endian gtest_main)
target_compile_options(endian_ut PUBLIC -pthread)
target_link_options(endian_ut PUBLIC -pthread)

include(GoogleTest)

gtest_discover_tests(endian_ut)
