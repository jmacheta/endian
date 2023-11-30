#include <ecpp/endian.hpp>
#include <gtest/gtest.h>

using ecpp::hton;
using ecpp::ntoh;

TEST(ECPP_Endian, NtohHtonByValue) {

  constexpr std::uint8_t x8 = 0x12U;
  constexpr std::uint8_t x8h = ntoh(x8);
  static_assert(x8h == x8);
  EXPECT_EQ(x8h, x8);
  EXPECT_EQ(hton(x8h), x8);

  constexpr std::uint16_t x16 = 0x1122U;
  constexpr std::uint16_t x16h = ntoh(x16);
  if constexpr(std::endian::native == std::endian::big) {
    EXPECT_EQ(x16, x16h);
  } else {
    EXPECT_EQ(x16h, 0x2211U);
  }
  EXPECT_EQ(hton(x16h), x16);

  constexpr std::uint32_t x32 = 0x11223344U;
  constexpr std::uint32_t x32h = ntoh(x32);
  if constexpr(std::endian::native == std::endian::big) {
    EXPECT_EQ(x32, x32h);
  } else {
    EXPECT_EQ(x32h, 0x44332211U);
  }
  EXPECT_EQ(hton(x32h), x32);

  constexpr std::uint64_t x64 = 0x1122334455667788U;
  constexpr std::uint64_t x64h = ntoh(x64);
  if constexpr(std::endian::native == std::endian::big) {
    EXPECT_EQ(x64, x64h);
  } else {
    EXPECT_EQ(x64h, 0x8877665544332211U);
  }
  EXPECT_EQ(hton(x64h), x64);
}
