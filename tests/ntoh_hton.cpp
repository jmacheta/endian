#include <ecpp/endian.hpp>
#include <gtest/gtest.h>


TEST(ECPP_Endian, NtohHtonByValue) {
    using ecpp::hton;
    using ecpp::ntoh;

    constexpr std::uint8_t x8  = 0x12U;
    constexpr std::uint8_t x8h = ntoh(x8);
    static_assert(x8h == x8);
    EXPECT_EQ(x8h, x8);
    EXPECT_EQ(hton(x8h), x8);


    constexpr std::uint16_t x16  = 0x1122U;
    constexpr std::uint16_t x16h = ntoh(x16);
    if constexpr (std::endian::native == std::endian::big) {
        EXPECT_EQ(x16, x16h);
    } else {
        EXPECT_EQ(x16h, 0x2211U);
    }
    EXPECT_EQ(hton(x16h), x16);


    constexpr std::uint32_t x32  = 0x11223344U;
    constexpr std::uint32_t x32h = ntoh(x32);
    if constexpr (std::endian::native == std::endian::big) {
        EXPECT_EQ(x32, x32h);
    } else {
        EXPECT_EQ(x32h, 0x44332211U);
    }
    EXPECT_EQ(hton(x32h), x32);


    constexpr std::uint64_t x64  = 0x1122334455667788U;
    constexpr std::uint64_t x64h = ntoh(x64);
    if constexpr (std::endian::native == std::endian::big) {
        EXPECT_EQ(x64, x64h);
    } else {
        EXPECT_EQ(x64h, 0x8877665544332211U);
    }
    EXPECT_EQ(hton(x64h), x64);
}


TEST(ECPP_Endian, NtohDecodeFromMemory) {
    using ecpp::ntoh;

    constexpr std::uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}; // Treat array as it would represent a number in Network endianness

    std::uint8_t x8h = ntoh<decltype(x8h)>(data);
    EXPECT_EQ(x8h, 0x11);

    std::uint16_t x16h = ntoh<decltype(x16h)>(data);
    EXPECT_EQ(x16h, 0x1122U);


    std::uint32_t x32h = ntoh<decltype(x32h)>(data);
    EXPECT_EQ(x32h, 0x11223344U);


    std::uint64_t x64h = ntoh<decltype(x64h)>(data);
    EXPECT_EQ(x64h, 0x1122334455667788U);
}


TEST(ECPP_Endian, NtohEncodeToMemory) {
    using ecpp::ntoh;

    std::uint8_t x8 = 0x11U;
    std::uint8_t x8o[10]{};
    ntoh(x8, x8o);
    EXPECT_EQ(x8o[0], 0x11);
    EXPECT_EQ(x8o[1], 0);


    std::uint16_t x16 = 0x1122U;
    std::uint8_t  x16o[10]{};
    ntoh(x16, x16o);
    EXPECT_EQ(x16o[0], 0x11);
    EXPECT_EQ(x16o[1], 0x22);
    EXPECT_EQ(x16o[2], 0);


    std::uint32_t x32 = 0x11223344U;
    std::uint8_t  x32o[10]{};
    ntoh(x32, x32o);
    EXPECT_EQ(x32o[0], 0x11);
    EXPECT_EQ(x32o[1], 0x22);
    EXPECT_EQ(x32o[2], 0x33);
    EXPECT_EQ(x32o[3], 0x44);
    EXPECT_EQ(x16o[4], 0);


    std::uint64_t x64 = 0x1122334455667788U;
    std::uint8_t  x64o[10]{};
    ntoh(x64, x64o);
    EXPECT_EQ(x64o[0], 0x11);
    EXPECT_EQ(x64o[1], 0x22);
    EXPECT_EQ(x64o[2], 0x33);
    EXPECT_EQ(x64o[3], 0x44);
    EXPECT_EQ(x64o[4], 0x55);
    EXPECT_EQ(x64o[5], 0x66);
    EXPECT_EQ(x64o[6], 0x77);
    EXPECT_EQ(x64o[7], 0x88);
    EXPECT_EQ(x16o[8], 0);
}


TEST(ECPP_Endian, HtonEncodeToMemory) {
    using ecpp::hton;

    std::uint8_t x8 = 0x11U;
    std::uint8_t x8o[10]{};
    hton(x8, x8o);
    EXPECT_EQ(x8o[0], 0x11);
    EXPECT_EQ(x8o[1], 0);


    std::uint16_t x16 = 0x1122U;
    std::uint8_t  x16o[10]{};
    hton(x16, x16o);
    EXPECT_EQ(x16o[0], 0x11);
    EXPECT_EQ(x16o[1], 0x22);
    EXPECT_EQ(x16o[2], 0);


    std::uint32_t x32 = 0x11223344U;
    std::uint8_t  x32o[10]{};
    hton(x32, x32o);
    EXPECT_EQ(x32o[0], 0x11);
    EXPECT_EQ(x32o[1], 0x22);
    EXPECT_EQ(x32o[2], 0x33);
    EXPECT_EQ(x32o[3], 0x44);
    EXPECT_EQ(x16o[4], 0);


    std::uint64_t x64 = 0x1122334455667788U;
    std::uint8_t  x64o[10]{};
    hton(x64, x64o);
    EXPECT_EQ(x64o[0], 0x11);
    EXPECT_EQ(x64o[1], 0x22);
    EXPECT_EQ(x64o[2], 0x33);
    EXPECT_EQ(x64o[3], 0x44);
    EXPECT_EQ(x64o[4], 0x55);
    EXPECT_EQ(x64o[5], 0x66);
    EXPECT_EQ(x64o[6], 0x77);
    EXPECT_EQ(x64o[7], 0x88);
    EXPECT_EQ(x16o[8], 0);
}


TEST(ECPP_Endian, HtonDecodeFromMemory) {
    using ecpp::hton;
    using std::byte;
    constexpr std::uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}; // Treat array as it would represent a number in Network endianness


    std::uint8_t x8h = hton<decltype(x8h)>(data);
    EXPECT_EQ(x8h, 0x11);
    std::uint8_t x8 = hton<decltype(x8)>(data);
    EXPECT_EQ(x8, x8h);


    constexpr std::uint16_t x16h = hton<decltype(x16h)>(data);
    EXPECT_EQ(x16h, 0x1122U);


    std::uint32_t x32h = hton<decltype(x32h)>(data);
    EXPECT_EQ(x32h, 0x11223344U);


    std::uint64_t x64h = hton<decltype(x64h)>(data);
    EXPECT_EQ(x64h, 0x1122334455667788U);
}
