#pragma once

#include <stdext/stream.h>
#include <stdext/traits.h>

#if STDEXT_ARCH_X86 && !STDEXT_COMPILER_GCC
#include <immintrin.h>
#endif


#if STDEXT_LITTLE_ENDIAN
#define STDEXT_BYTE_ORDER_NATIVE little_endian
#elif STDEXT_BIG_ENDIAN
#define STDEXT_BYTE_ORDER_NATIVE big_endian
#elif STDEXT_PDP_ENDIAN
#define STDEXT_BYTE_ORDER_NATIVE pdp_endian
#else
#error Unknown architecture
#endif


namespace stdext
{
    enum class byte_order
    {
        little_endian,
        big_endian,
        pdp_endian,

        native_endian = STDEXT_BYTE_ORDER_NATIVE
    };

    template <class T, byte_order ByteOrder, STDEXT_REQUIRES(std::is_integral_v<T>)>
    class endian_int;

    using int8_le = endian_int<int8_t, byte_order::little_endian>;
    using int16_le = endian_int<int16_t, byte_order::little_endian>;
    using int32_le = endian_int<int32_t, byte_order::little_endian>;
    using int64_le = endian_int<int64_t, byte_order::little_endian>;

    using uint8_le = endian_int<uint8_t, byte_order::little_endian>;
    using uint16_le = endian_int<uint16_t, byte_order::little_endian>;
    using uint32_le = endian_int<uint32_t, byte_order::little_endian>;
    using uint64_le = endian_int<uint64_t, byte_order::little_endian>;

    using int8_be = endian_int<int8_t, byte_order::big_endian>;
    using int16_be = endian_int<int16_t, byte_order::big_endian>;
    using int32_be = endian_int<int32_t, byte_order::big_endian>;
    using int64_be = endian_int<int64_t, byte_order::big_endian>;

    using uint8_be = endian_int<uint8_t, byte_order::big_endian>;
    using uint16_be = endian_int<uint16_t, byte_order::big_endian>;
    using uint32_be = endian_int<uint32_t, byte_order::big_endian>;
    using uint64_be = endian_int<uint64_t, byte_order::big_endian>;

    using int8_pdp = endian_int<int8_t, byte_order::pdp_endian>;
    using int16_pdp = endian_int<int16_t, byte_order::pdp_endian>;
    using int32_pdp = endian_int<int32_t, byte_order::pdp_endian>;
    using int64_pdp = endian_int<int64_t, byte_order::pdp_endian>;

    using uint8_pdp = endian_int<uint8_t, byte_order::pdp_endian>;
    using uint16_pdp = endian_int<uint16_t, byte_order::pdp_endian>;
    using uint32_pdp = endian_int<uint32_t, byte_order::pdp_endian>;
    using uint64_pdp = endian_int<uint64_t, byte_order::pdp_endian>;

    namespace detail
    {
        template <byte_order order, class T> struct endian;
    }

    template <byte_order order, class T, STDEXT_REQUIRES(std::is_integral_v<T>)>
    constexpr T swap(T v) noexcept
    {
        if constexpr (order == byte_order::native_endian)
            return v;

        using sized_t = equivalent_sized_type_t<T>;
        return T(detail::endian<order, sized_t>::swap(sized_t(v)));
    }

    template <byte_order order, class T, STDEXT_REQUIRES(std::is_integral_v<T>)>
    T read(input_stream& s)
    {
        using sized_t = equivalent_sized_type_t<T>;
        return T(swap<order>(s.read<sized_t>()));
    }

    template <byte_order order, class T, STDEXT_REQUIRES(std::is_integral_v<T>)>
    size_t read(input_stream& s, T* buffer, size_t count)
    {
        count = s.read(buffer, count);
        if constexpr (order != byte_order::native_endian)
        {
            for (size_t n = 0; n < count; ++n)
                buffer[n] = swap<order>(buffer[n]);
        }
        return count;
    }

    template <byte_order order, class T, size_t length, STDEXT_REQUIRES(std::is_integral_v<T>)>
    size_t read(input_stream& s, T (&buffer)[length])
    {
        using sized_t = equivalent_sized_type_t<T>;
        return detail::endian<order, sized_t>::read(s, buffer, length);
    }

    template <byte_order order, class T, STDEXT_REQUIRES(std::is_integral_v<T>)>
    void write(output_stream& s, T v)
    {
        s.write(swap<order>(v));
    }

    template <byte_order order, class T, STDEXT_REQUIRES(std::is_integral_v<T>)>
    size_t write(output_stream& s, const T* buffer, size_t count)
    {
        if constexpr (order == byte_order::native_endian)
            return s.write(buffer, count);

        size_t n = 0;
        for (; n < count; ++n)
        {
            auto v = swap<order>(buffer[n]);
            if (s.write(&v, 1) == 0)
                break;
        }
        return n;
    }

    template <byte_order order, class T, size_t length, STDEXT_REQUIRES(std::is_integral_v<T>)>
    size_t write(output_stream& s, const T (&buffer)[length])
    {
        return write(s, buffer, length);
    }

    namespace detail
    {
        template <byte_order order, class T>
        struct endian
        {
            static constexpr T swap(T v)
            {
                return endian<order, std::make_unsigned_t<T>>::swap(v);
            }
        };

        template <class T>
        struct endian<byte_order::native_endian, T>
        {
            static constexpr T swap(T v)
            {
                return v;
            }
        };

        template <byte_order order>
        struct endian<order, uint8_t>
        {
            static constexpr uint8_t swap(uint8_t v)
            {
                return v;
            }
        };

        template <>
        struct endian<byte_order::native_endian, uint8_t>
        {
            static constexpr uint8_t swap(uint8_t v)
            {
                return v;
            }
        };

#if STDEXT_LITTLE_ENDIAN
        template <>
        struct endian<byte_order::big_endian, uint16_t>
        {
            static constexpr uint16_t swap(uint16_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap16(v);
#else
                return uint16_t(v << 8 | v >> 8);
#endif
            }
        };

        template <>
        struct endian<byte_order::big_endian, uint32_t>
        {
            static constexpr uint32_t swap(uint32_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap32(v);
#else
                return uint32_t(
                    (v & 0x000000FF) << 24
                    | (v & 0x0000FF00) << 8
                    | (v & 0x00FF0000) >> 8
                    | (v & 0xFF000000) >> 24
                );
#endif
            }
        };

        template <>
        struct endian<byte_order::big_endian, uint64_t>
        {
            static constexpr uint64_t swap(uint64_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap64(v);
#else
                return uint64_t(
                    (v & 0x00000000'000000FF) << 56
                    | (v & 0x00000000'0000FF00) << 40
                    | (v & 0x00000000'00FF0000) << 24
                    | (v & 0x00000000'FF000000) << 8
                    | (v & 0x000000FF'00000000) >> 8
                    | (v & 0x0000FF00'00000000) >> 24
                    | (v & 0x00FF0000'00000000) >> 40
                    | (v & 0xFF000000'00000000) >> 56
                );
#endif
            }
        };

        template <>
        struct endian<byte_order::pdp_endian, uint16_t>
        {
            static constexpr uint16_t swap(uint16_t v)
            {
                return v;
            }
        };

        template <>
        struct endian<byte_order::pdp_endian, uint32_t>
        {
            static constexpr uint32_t swap(uint32_t v)
            {
                return uint32_t(v << 16 | v >> 16);
            }
        };

        template <>
        struct endian<byte_order::pdp_endian, uint64_t>
        {
            static constexpr uint64_t swap(uint64_t v)
            {
                return uint64_t(
                    (v & 0x00000000'0000FFFF) << 48
                    | (v & 0x00000000'FFFF0000) << 16
                    | (v & 0x0000FFFF'00000000) >> 16
                    | (v & 0xFFFF0000'00000000) >> 48
                );
            }
        };
#endif

#if STDEXT_BIG_ENDIAN
        template <>
        struct endian<byte_order::little_endian, uint16_t>
        {
            static constexpr uint16_t swap(uint16_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap16(v);
#else
                return uint16_t(v << 8 | v >> 8);
#endif
            }
        };

        template <>
        struct endian<byte_order::little_endian, uint32_t>
        {
            static constexpr uint32_t swap(uint32_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap32(v);
#else
                return uint32_t(
                    (v & 0x000000FF) << 24
                    | (v & 0x0000FF00) << 8
                    | (v & 0x00FF0000) >> 8
                    | (v & 0xFF000000) >> 24
                );
#endif
            }
        };

        template <>
        struct endian<byte_order::little_endian, uint64_t>
        {
            static constexpr uint64_t swap(uint64_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap64(v);
#else
                return uint64_t(
                    (v & 0x00000000'000000FF) << 56
                    | (v & 0x00000000'0000FF00) << 40
                    | (v & 0x00000000'00FF0000) << 24
                    | (v & 0x00000000'FF000000) << 8
                    | (v & 0x000000FF'00000000) >> 8
                    | (v & 0x0000FF00'00000000) >> 24
                    | (v & 0x00FF0000'00000000) >> 40
                    | (v & 0xFF000000'00000000) >> 56
                );
#endif
            }
        };

        template <>
        struct endian<byte_order::pdp_endian, uint16_t>
        {
            static constexpr uint16_t swap(uint16_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap16(v);
#else
                return uint16_t(v << 8 | v >> 8);
#endif
            }
        };

        template <>
        struct endian<byte_order::pdp_endian, uint32_t>
        {
            static constexpr uint32_t swap(uint32_t v)
            {
                return uint32_t(
                    (v & 0x00FF00FF) << 8
                    | (v & 0xFF00FF00) >> 8
                );
            }
        };

        template <>
        struct endian<byte_order::pdp_endian, uint64_t>
        {
            static constexpr uint64_t swap(uint64_t v)
            {
                return uint64_t(
                    (v & 0x00FF00FF'00FF00FF) << 8
                    | (v & 0xFF00FF00'FF00FF00) >> 8
                );
            }
        };
#endif

#if STDEXT_PDP_ENDIAN
        template <>
        struct endian<byte_order::little_endian, uint16_t>
        {
            static constexpr uint16_t swap(uint16_t v)
            {
                return v;
            }
        };

        template <>
        struct endian<byte_order::little_endian, uint32_t>
        {
            static constexpr uint32_t swap(uint32_t v)
            {
                return uint32_t(v << 16 | v >> 16);
            }
        };

        template <>
        struct endian<byte_order::little_endian, uint64_t>
        {
            static constexpr uint64_t swap(uint64_t v)
            {
                return uint64_t(
                    (v & 0x00000000'0000FFFF) << 48
                    | (v & 0x00000000'FFFF0000) << 16
                    | (v & 0x0000FFFF'00000000) >> 16
                    | (v & 0xFFFF0000'00000000) >> 48
                );
            }
        };

        template <>
        struct endian<byte_order::big_endian, uint16_t>
        {
            static constexpr uint16_t swap(uint16_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap16(v);
#else
                return uint16_t(v << 8 | v >> 8);
#endif
            }
        };

        template <>
        struct endian<byte_order::big_endian, uint32_t>
        {
            static constexpr uint32_t swap(uint32_t v)
            {
                return uint32_t(
                    (v & 0x00FF00FF) << 8
                    | (v & 0xFF00FF00) >> 8
                );
            }
        };

        template <>
        struct endian<byte_order::big_endian, uint64_t>
        {
            static constexpr uint64_t swap(uint64_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap64(v);
#else
                return uint64_t(
                    (v & 0x00FF00FF'00FF00FF) << 8
                    | (v & 0xFF00FF00'FF00FF00) >> 8
                );
#endif
            }
        };
#endif
    }

    template <class T, byte_order ByteOrder, STDEXT_REQUIRED(std::is_integral_v<T>)>
    class endian_int
    {
    public:
        endian_int() noexcept = default;
        constexpr endian_int(T value) noexcept : value_(swap<ByteOrder>(value)) { }
        constexpr endian_int& operator = (T value) { value_ = value; return *this; }

        friend constexpr bool operator == (const endian_int& lhs, const endian_int& rhs) noexcept
        {
            return lhs.value == rhs.value;
        }

        friend constexpr bool operator != (const endian_int& lhs, const endian_int& rhs) noexcept
        {
            return lhs.value != rhs.value;
        }

        friend constexpr bool operator == (const endian_int& lhs, T rhs) noexcept
        {
            return lhs.get() == rhs;
        }

        friend constexpr bool operator == (T lhs, const endian_int& rhs) noexcept
        {
            return lhs == rhs.get();
        }

        friend constexpr bool operator != (const endian_int& lhs, T rhs) noexcept
        {
            return lhs.get() != rhs;
        }

        friend constexpr bool operator != (T lhs, const endian_int& rhs) noexcept
        {
            return lhs != rhs.get();
        }

        friend constexpr bool operator < (const endian_int& lhs, T rhs) noexcept
        {
            return lhs.get() < rhs;
        }

        friend constexpr bool operator < (T lhs, const endian_int& rhs) noexcept
        {
            return lhs < rhs.get();
        }

        friend constexpr bool operator > (const endian_int& lhs, T rhs) noexcept
        {
            return lhs.get() > rhs;
        }

        friend constexpr bool operator > (T lhs, const endian_int& rhs) noexcept
        {
            return lhs > rhs.get();
        }

        friend constexpr bool operator <= (const endian_int& lhs, T rhs) noexcept
        {
            return lhs.get() <= rhs;
        }

        friend constexpr bool operator <= (T lhs, const endian_int& rhs) noexcept
        {
            return lhs <= rhs.get();
        }

        friend constexpr bool operator >= (const endian_int& lhs, T rhs) noexcept
        {
            return lhs.get() >= rhs;
        }

        friend constexpr bool operator >= (T lhs, const endian_int& rhs) noexcept
        {
            return lhs >= rhs.get();
        }

        constexpr T get() const noexcept { return swap<ByteOrder>(value_); }
        constexpr operator T () const noexcept { return get(); }

    private:
        T value_;
    };

    inline namespace literals
    {
        inline namespace endian
        {
            // Given an input of 'ABCD', returns the integer value corresponding to the bytes
            // 'A', 'B', 'C', and 'D', in that order.
            constexpr uint32_t operator ""_4cc(const char* str, size_t size)
            {
                static_assert(byte_order::native_endian == byte_order::little_endian
                              || byte_order::native_endian == byte_order::big_endian
                              || byte_order::native_endian == byte_order::pdp_endian,
                              "Unknown edianness.");

                if (size != 4)
                    throw "_4cc literal must have four characters";

                return swap<byte_order::big_endian>(
                    uint32_t(str[0]) << 24
                    | uint32_t(str[1]) << 16
                    | uint32_t(str[2]) << 8
                    | uint32_t(str[3]));
            }

            constexpr uint16_t operator ""_le16(unsigned long long v)
            {
                return swap<byte_order::little_endian>(uint16_t(v));
            }

            constexpr uint16_t operator ""_be16(unsigned long long v)
            {
                return swap<byte_order::big_endian>(uint16_t(v));
            }

            constexpr uint16_t operator ""_pdp16(unsigned long long v)
            {
                return swap<byte_order::pdp_endian>(uint16_t(v));
            }

            constexpr uint32_t operator ""_le32(unsigned long long v)
            {
                return swap<byte_order::little_endian>(uint32_t(v));
            }

            constexpr uint32_t operator ""_be32(unsigned long long v)
            {
                return swap<byte_order::big_endian>(uint32_t(v));
            }

            constexpr uint32_t operator ""_pdp32(unsigned long long v)
            {
                return swap<byte_order::pdp_endian>(uint32_t(v));
            }

            constexpr uint64_t operator ""_le64(unsigned long long v)
            {
                return swap<byte_order::little_endian>(uint64_t(v));
            }

            constexpr uint64_t operator ""_be64(unsigned long long v)
            {
                return swap<byte_order::big_endian>(uint64_t(v));
            }

            constexpr uint64_t operator ""_pdp64(unsigned long long v)
            {
                return swap<byte_order::pdp_endian>(uint64_t(v));
            }
        }
    }
}

#undef STDEXT_BYTE_ORDER_NATIVE
