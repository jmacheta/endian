# endian
C++20 Endianness conversion library

[![C++ CI](https://github.com/jmacheta/endian/actions/workflows/test_action.yml/badge.svg)](https://github.com/jmacheta/endian/actions/workflows/test_action.yml)


## Installation

The easiest way is to use built-in CMake FetchContent:

```cmake
include(FetchContent)
FetchContent_Declare(
    ecpp_endian
    GIT_REPOSITORY https://github.com/jmacheta/endian.git
    GIT_TAG main
)

FetchContent_MakeAvailable(ecpp_endian)
```

Then add a dependency to your target:

```cmake
target_link_libraries(my_target PUBLIC ecpp::endian)
```

## Usage

todo