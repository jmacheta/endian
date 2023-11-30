#include <ecpp/endian.hpp>
#include <gtest/gtest.h>

using ecpp::byteswap;

TEST(ECPP_Endian, Byteswap) {
  constexpr std::uint8_t x8 = 0x12U;
  static_assert(byteswap(x8) == x8);
  EXPECT_EQ(byteswap(x8), x8);

  constexpr std::uint16_t x16 = 0x1122U;
  static_assert(byteswap(x16) == 0x2211U);
  EXPECT_EQ(byteswap(x16), 0x2211U);

  constexpr std::uint32_t x32 = 0x11223344U;
  static_assert(byteswap(x32) == 0x44332211U);
  EXPECT_EQ(byteswap(x32), 0x44332211U);

  constexpr std::uint64_t x64 = 0x1122334455667788U;
  static_assert(byteswap(x64) == 0x8877665544332211U);
  EXPECT_EQ(byteswap(x64), 0x8877665544332211U);

  // Now do the same for signed integral types
  constexpr std::int8_t i8 = 0x12;
  static_assert(byteswap(i8) == i8);
  EXPECT_EQ(byteswap(i8), i8);

  constexpr std::int16_t i16 = 0x1122;
  static_assert(byteswap(i16) == 0x2211);
  EXPECT_EQ(byteswap(i16), 0x2211);

  constexpr std::int32_t i32 = 0x11223344;
  static_assert(byteswap(i32) == 0x44332211);
  EXPECT_EQ(byteswap(i32), 0x44332211);

  constexpr std::int64_t i64 = 0x1122334455667788;
  static_assert(static_cast<std::uint64_t>(byteswap(i64)) == 0x8877665544332211U);
  EXPECT_EQ(static_cast<std::uint64_t>(byteswap(i64)), 0x8877665544332211U);
}
