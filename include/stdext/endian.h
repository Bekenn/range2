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

    namespace detail
    {
        template <byte_order order, class T> struct endian;
    }

    template <byte_order order, class T, REQUIRES(::std::is_integral_v<T>)>
    constexpr T swap(T v) noexcept
    {
        if constexpr (order == byte_order::native_endian)
            return v;

        using sized_t = equivalent_sized_type_t<T>;
        return T(detail::endian<order, sized_t>::swap(sized_t(v)));
    }

    template <byte_order order, class T, REQUIRES(::std::is_integral_v<T>)>
    T read(input_stream& s)
    {
        using sized_t = equivalent_sized_type_t<T>;
        return T(swap<order>(s.read<sized_t>()));
    }

    template <byte_order order, class T, REQUIRES(::std::is_integral_v<T>)>
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

    template <byte_order order, class T, size_t length, REQUIRES(::std::is_integral_v<T>)>
    size_t read(input_stream& s, T (&buffer)[length])
    {
        using sized_t = equivalent_sized_type_t<T>;
        return detail::endian<order, sized_t>::read(s, buffer, length);
    }

    template <byte_order order, class T, REQUIRES(::std::is_integral_v<T>)>
    void write(output_stream& s, T v)
    {
        s.write(swap<order>(v));
    }

    template <byte_order order, class T, REQUIRES(::std::is_integral_v<T>)>
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

    template <byte_order order, class T, size_t length, REQUIRES(::std::is_integral_v<T>)>
    size_t write(output_stream& s, const T (&buffer)[length])
    {
        return write(s, buffer, length);
    }

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

                if constexpr (byte_order::native_endian == byte_order::little_endian)
                {
                    return uint32_t(str[3]) << 24
                        | uint32_t(str[2]) << 16
                        | uint32_t(str[1]) << 8
                        | uint32_t(str[0]);
                }
                if constexpr (byte_order::native_endian == byte_order::big_endian)
                {
                    return uint32_t(str[0]) << 24
                        | uint32_t(str[1]) << 16
                        | uint32_t(str[2]) << 8
                        | uint32_t(str[0]);
                }
                if constexpr (byte_order::native_endian == byte_order::pdp_endian)
                {
                    return uint32_t(str[1]) << 24
                        | uint32_t(str[0]) << 16
                        | uint32_t(str[3]) << 8
                        | uint32_t(str[2]);
                }
            }
        }
    }

    namespace detail
    {
        template <byte_order order, class T>
        struct endian
        {
            static T swap(T v)
            {
                return endian<order, ::std::make_unsigned_t<T>>::swap(v);
            }
        };

        template <class T>
        struct endian<byte_order::native_endian, T>
        {
            static T swap(T v)
            {
                return v;
            }
        };

        template <byte_order order>
        struct endian<order, uint8_t>
        {
            static uint8_t swap(uint8_t v)
            {
                return v;
            }
        };

        template <>
        struct endian<byte_order::native_endian, uint8_t>
        {
            static uint8_t swap(uint8_t v)
            {
                return v;
            }
        };

#if !STDEXT_LITTLE_ENDIAN
        template <>
        struct endian<byte_order::little_endian, uint16_t>
        {
            static uint16_t swap(uint16_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap16(v);
#else
#error Implement me
#endif
            }
        };

        template <>
        struct endian<byte_order::little_endian, uint32_t>
        {
            static uint32_t swap(uint32_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap32(v);
#else
#error Implement me
#endif
            }
        };

        template <>
        struct endian<byte_order::little_endian, uint64_t>
        {
            static uint64_t swap(uint64_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap64(v);
#else
#error Implement me
#endif
            }
        };
#endif

#if !STDEXT_BIG_ENDIAN
        template <>
        struct endian<byte_order::big_endian, uint16_t>
        {
            static uint16_t swap(uint16_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap16(v);
#elif STDEXT_ARCH_X86
                return _load_be_u16(&v);
#else
#error Implement me
#endif
            }
        };

        template <>
        struct endian<byte_order::big_endian, uint32_t>
        {
            static uint32_t swap(uint32_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap32(v);
#elif STDEXT_ARCH_X86
                return _load_be_u32(&v);
#else
#error Implement me
#endif
            }
        };

        template <>
        struct endian<byte_order::big_endian, uint64_t>
        {
            static uint64_t swap(uint64_t v)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap64(v);
#elif STDEXT_ARCH_X86
                return _load_be_u64(&v);
#else
#error Implement me
#endif
            }
        };
#endif

#if !STDEXT_PDP_ENDIAN
        template <>
        struct endian<byte_order::pdp_endian, uint16_t>
        {
            static uint16_t swap(uint16_t v)
            {
                return endian<byte_order::little_endian, uint16_t>::swap(v);
            }
        };

        template <>
        struct endian<byte_order::pdp_endian, uint32_t>
        {
            static uint32_t swap(uint32_t v)
            {
#if STDEXT_LITTLE_ENDIAN
                return (uint32_t(endian<byte_order::little_endian, uint16_t>::swap(uint16_t(v))) << 16)
                    | endian<byte_order::little_endian, uint16_t>::swap(uint16_t(v >> 16));
#elif STDEXT_BIG_ENDIAN
                return (uint32_t(endian<byte_order::little_endian, uint16_t>::swap(uint16_t(v >> 16))) << 16)
                    | endian<byte_order::little_endian, uint16_t>::swap(uint16_t(v));
#else
#error Implement me
#endif
            }
        };

        template <>
        struct endian<byte_order::pdp_endian, uint64_t>
        {
            static uint64_t swap(uint64_t v)
            {
#if STDEXT_LITTLE_ENDIAN
                return (uint64_t(endian<byte_order::pdp_endian, uint32_t>::swap(uint32_t(v))) << 32)
                    | endian<byte_order::pdp_endian, uint32_t>::swap(uint32_t(v >> 32));
#elif STDEXT_BIG_ENDIAN
                return (uint64_t(endian<byte_order::pdp_endian, uint32_t>::swap(uint32_t(v >> 32))) << 32)
                    | endian<byte_order::pdp_endian, uint32_t>::swap(uint32_t(v));
#else
#error Implement me
#endif
            }
        };
#endif
    }
}

#undef STDEXT_BYTE_ORDER_NATIVE
