#ifndef ECPP_ENDIAN_HPP__
#define ECPP_ENDIAN_HPP__

#include <algorithm>
#include <array>
#include <bit>
#include <concepts>
#include <cstring>
namespace ecpp {

#if __cpp_lib_byteswap >= 202110L
using std::byteswap;
#elif __cpp_lib_bit_cast >= 201806L
template <std::integral T> [[nodiscard]] constexpr T byteswap(T value) noexcept {
  auto value_representation = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
  std::ranges::reverse(value_representation);
  return std::bit_cast<T>(value_representation);
}
#else
template <std::integral T> [[nodiscard]] constexpr T byteswap(T value) noexcept {
  alignas(T) std::array<std::byte, sizeof(T)> value_representation;
  std::memcpy(value_representation.data(), &value, sizeof(value));
  std::ranges::reverse_copy(value_representation, reinterpret_cast<std::byte *>(&value));
  return value;
}
#endif

/**
 * @brief Convert value from network byte order to host byte order
 *
 * @tparam T type that sarisfies std::integral, i.e., T is an integer type. The
 * program is ill-formed if T has padding bits
 * @param[in] n integer value in network byte order
 * @return An integer value of type T whose object representation comprises the
 * bytes of that of n in host byte order
 */
template <std::integral T> [[nodiscard]] constexpr T ntoh(T n) noexcept {
  return (std::endian::native == std::endian::big) ? n : byteswap(n);
}

/**
 * @brief Convert value from host byte order to network byte order
 *
 * @tparam T type that sarisfies std::integral, i.e., T is an integer type. The
 * program is ill-formed if T has padding bits
 * @param[in] n integer value in host byte order
 * @return An integer value of type T whose object representation comprises the
 * bytes of that of n in network byte order
 */
template <std::integral T> [[nodiscard]] constexpr T hton(T n) noexcept { return ntoh(n); }

} // namespace ecpp

#endif
