
# endian

C++20 Endianness conversion library

[![C++ CI](https://github.com/jmacheta/endian/actions/workflows/test_action.yml/badge.svg)](https://github.com/jmacheta/endian/actions/workflows/test_action.yml)

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/a1a915a6d2bc42f99dfea02067485869)](https://app.codacy.com/gh/jmacheta/endian/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)

## Installation

The easiest way is to use built-in CMake FetchContent:

```cmake
include(FetchContent)
FetchContent_Declare(ecpp_endian URL https://github.com/jmacheta/endian/tarball/latest)

FetchContent_MakeAvailable(ecpp_endian)
```

Then add a dependency to your target:

```cmake
target_link_libraries(my_target PUBLIC ecpp::endian)
```

## Usage

### byteswap - unconditionally reserse byte order in integral types

```cpp
#include <ecpp/endian.hpp>

uint64_t a = 0x1122'3344'5566'7788U;
static_assert(0x8877'6655'4433'2211U == ecpp::byteswap(a));

```

### hton / ntoh - convert endiannes from/to network endianness (big-endiann)

```cpp
// On little endian systems:
uint64_t a = 0x1122'3344'5566'7788U;
static_assert(0x8877'6655'4433'2211U == ecpp::hton(a));
static_assert(a == ecpp::ntoh(ecpp::hton(a)));

// On big endian systems:
uint64_t a = 0x1122'3344'5566'7788U;
static_assert(a == ecpp::hton(a));
static_assert(a == ecpp::ntoh(ecpp::hton(a)));
```

## Limitations

While every function is marked as `constexpr`, in C++ standards prior to C++20, this might be untrue due to the non-constexpr `std::memcpy`. I have checked many clang/gcc versions, and this does not seem to be an issue, but it's worth noting.
