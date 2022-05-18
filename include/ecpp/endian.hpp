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
        /**
         * @brief Returns n
         *
         * This function is implemented to make sure that every integral type may be handled by byteswap
         * @param[in] n single byte unsigned integer
         * @return n
         */
        constexpr std::uint8_t bswap(std::uint8_t n) noexcept {
            return n;
        }

        /**
         * @brief Reverses the bytes in the given integer value n
         *
         * @param[in] n two byte unsigned integer
         * @return n with reversed byte order
         */
        constexpr std::uint16_t bswap(std::uint16_t n) noexcept {
            return static_cast<std::uint16_t>((n << 8U) | (n >> 8U));
        }

        /**
         * @brief Reverses the bytes in the given integer value n
         *
         * @param[in] n four byte unsigned integer
         * @return n with reversed byte order
         */
        constexpr std::uint32_t bswap(std::uint32_t n) noexcept {
            return static_cast<std::uint32_t>(bswap(static_cast<std::uint16_t>(n))) << 16U | static_cast<std::uint32_t>(bswap(static_cast<std::uint16_t>(n >> 16U)));
        }

        /**
         * @brief Reverses the bytes in the given integer value n
         *
         * @param[in] n eight byte unsigned integer
         * @return n with reversed byte order
         */
        constexpr std::uint64_t bswap(std::uint64_t n) noexcept {
            return static_cast<std::uint64_t>(bswap(static_cast<std::uint32_t>(n))) << 32U | static_cast<std::uint64_t>(bswap(static_cast<std::uint32_t>(n >> 32U)));
        }


        /// Concept that is true for every contiguous iterator that iterates over values with size of 1 byte
        template<typename T>
        concept ContiguousByteIt = std::contiguous_iterator<T> &&(1 == sizeof(typename std::iterator_traits<T>::value_type));

        /// Concept that is true for every output and contiguous iterator that iterates over values with size of 1 byte
        template<typename T>
        concept ContiguousOutputByteIt = ContiguousByteIt<T> && std::output_iterator<T, typename std::iterator_traits<T>::value_type>;
    } // namespace impl


    /**
     * @brief Reverses the bytes in the given integer value n
     *
     * @tparam T type that sarisfies std::integral, i.e., T is an integer type. The program is ill-formed if T has padding bits
     * @param[in] n integer value
     * @note this function will use std::byteswap if possible
     * @return An integer value of type T whose object representation comprises the bytes of that of n in reversed order
     */
    template<std::integral T> [[nodiscard]] constexpr T byteswap(T n) noexcept {
#ifdef __cpp_lib_byteswap
        return std::byteswap(n);
#else
        if constexpr (1 == sizeof(n)) {
            return static_cast<T>(impl::bswap(static_cast<std::uint8_t>(n)));
        } else if constexpr (2 == sizeof(n)) {
            return static_cast<T>(impl::bswap(static_cast<std::uint16_t>(n)));
        } else if constexpr (4 == sizeof(n)) {
            return static_cast<T>(impl::bswap(static_cast<std::uint32_t>(n)));
        } else if constexpr (8 == sizeof(n)) {
            return static_cast<T>(impl::bswap(static_cast<std::uint64_t>(n)));
        } else {
            static_assert(0 == sizeof(n), "ecpp::byteswap is not implemented for type of this size");
        }
#endif
    }


    /**
     * @brief Convert value from network byte order to host byte order
     *
     * @tparam T type that sarisfies std::integral, i.e., T is an integer type. The program is ill-formed if T has padding bits
     * @param[in] n integer value in network byte order
     * @return An integer value of type T whose object representation comprises the bytes of that of n in host byte order
     */
    template<std::integral T> [[nodiscard]] constexpr T ntoh(T n) noexcept {
        return (std::endian::native == std::endian::big) ? n : byteswap(n);
    }

    /**
     * @brief Returns value of type T by copying sizeof(T) bytes from src and converting from network byte order to host byte order
     *
     * @tparam T type that satisfies std::integral, i.e., T is an integer type. The program is ill-formed if T has padding bits
     * @tparam CByteIt type that satisfies impl::ContiguousByteIt, i.e., CByteIt is contiguous iterator, that iterates over values with size of 1
     * @param[in] src the beginning of the memory region in network byte order, which should be converted to T. src must point to memory region of at least sizeof(T) size, otherwise the behaviour is
     * undefined
     * @return An integer value of type T whose object representation comprises the bytes of src converted to host byte order
     */
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


    /**
     * @brief Converts n from network byte order to host byte order, and copies it into dst
     *
     * @tparam T type that satisfies std::integral, i.e., T is an integer type. The program is ill-formed if T has padding bits
     * @tparam COutByteIt type that satisfies impl::ContiguousOutputByteIt, i.e., COutByteIt is output and contiguous iterator, that iterates over values with size of 1
     * @param[in] n integer value in network byte order
     * @param[in,out] dst the beginning of the memory region to store n. dst must point to memory region of at least sizeof(T) size, otherwise the behaviour is
     * undefined
     */
    template<std::integral T, impl::ContiguousOutputByteIt COutByteIt> constexpr void ntoh(T n, COutByteIt dst) noexcept {
        using value_type = std::remove_cvref_t<T>;

        auto v = ntoh(n);

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

    /**
     * @brief Convert value from host byte order to network byte order
     *
     * @tparam T type that sarisfies std::integral, i.e., T is an integer type. The program is ill-formed if T has padding bits
     * @param[in] n integer value in host byte order
     * @return An integer value of type T whose object representation comprises the bytes of that of n in network byte order
     */
    template<std::integral T> [[nodiscard]] constexpr T hton(T n) noexcept {
        return ntoh(n);
    }


    /**
     * @brief Returns value of type T by copying sizeof(T) bytes from src and converting from host byte order to network byte order
     *
     * @tparam T type that satisfies std::integral, i.e., T is an integer type. The program is ill-formed if T has padding bits
     * @tparam CByteIt type that satisfies impl::ContiguousByteIt, i.e., CByteIt is contiguous iterator, that iterates over values with size of 1
     * @param[in] src the beginning of the memory region in host byte order, which should be converted to T. src must point to memory region of at least sizeof(T) size, otherwise the behaviour is
     * undefined
     * @return An integer value of type T whose object representation comprises the bytes of src converted to network byte order
     */
    template<std::integral T, std::contiguous_iterator CInputIt> [[nodiscard]] constexpr T hton(CInputIt src) noexcept {
        return ntoh<T, CInputIt>(src);
    }


    /**
     * @brief Converts n from host byte order to network byte order, and copies it into dst
     *
     * @tparam T type that satisfies std::integral, i.e., T is an integer type. The program is ill-formed if T has padding bits
     * @tparam COutByteIt type that satisfies impl::CContiguousOutputByteIt, i.e., COutByteIt is output and contiguous iterator, that iterates over values with size of 1
     * @param[in] n integer value in host byte order
     * @param[in,out] dst the beginning of the memory region to store n. dst must point to memory region of at least sizeof(T) size, otherwise the behaviour is
     * undefined
     */
    template<std::integral T, impl::ContiguousOutputByteIt COutByteIt> constexpr void hton(T n, COutByteIt dst) noexcept {
        return ntoh(n, dst);
    }


} // namespace ecpp


#endif