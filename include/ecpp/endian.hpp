#ifndef ECPP_ENDIAN_HPP__
#define ECPP_ENDIAN_HPP__

#include <algorithm>
#include <bit>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <type_traits>

namespace ecpp {

    namespace impl {
        constexpr std::uint8_t bswap(std::uint8_t v) noexcept {
            return v;
        }

        constexpr std::uint16_t bswap(std::uint16_t v) noexcept {
            return static_cast<std::uint16_t>((v << 8U) | (v >> 8U));
        }

        constexpr std::uint32_t bswap(std::uint32_t v) noexcept {
            return static_cast<std::uint32_t>(bswap(static_cast<std::uint16_t>(v))) << 16U | static_cast<std::uint32_t>(bswap(static_cast<std::uint16_t>(v >> 16U)));
        }

        constexpr std::uint64_t bswap(std::uint64_t v) noexcept {
            return static_cast<std::uint64_t>(bswap(static_cast<std::uint32_t>(v))) << 32U | static_cast<std::uint64_t>(bswap(static_cast<std::uint32_t>(v >> 32U)));
        }


        template<typename T>
        concept ContiguousByteIt = std::contiguous_iterator<T> &&(1 == sizeof(typename std::iterator_traits<T>::value_type));

        template<typename T>
        concept ContiguousOutputByteIt = ContiguousByteIt<T> && std::output_iterator<T, typename std::iterator_traits<T>::value_type>;
    } // namespace impl


    template<std::integral T> [[nodiscard]] constexpr T byteswap(T value) noexcept {
#ifdef __cpp_lib_byteswap
        return std::byteswap(value);
#else
        if constexpr (1 == sizeof(value)) {
            return static_cast<T>(impl::bswap(static_cast<std::uint8_t>(value)));
        } else if constexpr (2 == sizeof(value)) {
            return static_cast<T>(impl::bswap(static_cast<std::uint16_t>(value)));
        } else if constexpr (4 == sizeof(value)) {
            return static_cast<T>(impl::bswap(static_cast<std::uint32_t>(value)));
        } else if constexpr (8 == sizeof(value)) {
            return static_cast<T>(impl::bswap(static_cast<std::uint64_t>(value)));
        } else {
            static_assert(0 == sizeof(value), "ecpp::byteswap is not implemented for type of this size");
        }
#endif
    }


    template<std::integral T> [[nodiscard]] constexpr T ntoh(T value) noexcept {
        return (std::endian::native == std::endian::big) ? value : byteswap(value);
    }


    template<std::integral T, impl::ContiguousByteIt CByteIt> [[nodiscard]] constexpr T ntoh(CByteIt src) noexcept {
        using value_type = std::remove_cvref_t<T>;

#ifdef __cpp_lib_bit_cast
        // Thanks to constexpr bit_cast we can evaluate this expression during compilation

        using iterator_value_type = std::remove_cvref_t<typename std::iterator_traits<CByteIt>::value_type>;
        struct {
            alignas(value_type) iterator_value_type value[sizeof(value_type)];
        } tmp;
        std::copy_n(src, sizeof(value_type), std::begin(tmp.value));
        auto value = std::bit_cast<value_type>(tmp);
#else
        value_type value;
        std::memcpy(&value, &*src, sizeof(value)); // Because of memcpy, this expression will NOT be constant expression
#endif
        return (std::endian::native == std::endian::big) ? value : byteswap(value);
    }


    template<std::integral T, impl::ContiguousOutputByteIt COutByteIt> constexpr void ntoh(T value, COutByteIt dst) noexcept {
        using value_type = std::remove_cvref_t<T>;

        auto v = ntoh(value);

#ifdef __cpp_lib_bit_cast
        // Thanks to constexpr bit_cast we can evaluate this expression during compilation
        using iterator_value_type = std::remove_cvref_t<typename std::iterator_traits<COutByteIt>::value_type>;
        struct DST {
            alignas(value_type) iterator_value_type value[sizeof(value_type)];
        };
        auto tmp = std::bit_cast<DST>(v);

        std::copy_n(std::begin(tmp.value), sizeof(value_type), dst);
#else
        std::memcpy(&*dst, &v, sizeof(v));
#endif
    }


    template<std::integral T> [[nodiscard]] constexpr T hton(T value) noexcept {
        return ntoh(value);
    }


    template<std::integral T, std::contiguous_iterator CInputIt> [[nodiscard]] constexpr T hton(CInputIt src) noexcept {
        return ntoh<T, CInputIt>(src);
    }


    template<std::integral T, impl::ContiguousOutputByteIt COutByteIt> constexpr void hton(T value, COutByteIt dst) noexcept {
        return ntoh(value, dst);
    }


} // namespace ecpp


#endif