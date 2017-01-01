#pragma once

#include "traits.h"

#ifdef STDEXT_ARCH_X86
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
    T read(const void* p)
    {
        using sized_t = equivalent_sized_type_t<T>;
        return detail::endian<order, sized_t>::read(p);
    }

    template <byte_order order, class T, REQUIRES(::std::is_integral_v<T>)>
    void write(void* p, T v)
    {
        using sized_t = equivalent_sized_type_t<T>;
        return detail::endian<order, sized_t>::write(p, sized_t(v));
    }

    template <byte_order order, class T, REQUIRES(::std::is_integral_v<T>)>
    T swap(T v)
    {
        using sized_t = equivalent_sized_type_t<T>;
        return detail::endian<order, sized_t>::swap(sized_t(v));
    }

    namespace detail
    {
        template <byte_order order, class T>
        struct endian
        {
            static T read(const void* p)
            {
                return endian<order, ::std::make_unsigned_t<T>>::read(p);
            }

            static void write(void* p, T v)
            {
                endian<order, ::std::make_unsigned_t<T>>::write(p, v);
            }

            static T swap(T v)
            {
                return endian<order, ::std::make_unsigned_t<T>>::swap(v);
            }
        };

        template <class T>
        struct endian<byte_order::native_endian, T>
        {
            static T read(const void* p)
            {
                return *static_cast<const T*>(p);
            }

            static void write(void* p, T v)
            {
                *static_cast<T*>(p) = v;
            }

            static T swap(T v)
            {
                return v;
            }
        };

        template <byte_order order>
        struct endian<order, uint8_t>
        {
            static uint8_t read(const void* p)
            {
                return *static_cast<const uint8_t*>(p);
            }

            static void write(void* p, uint8_t v)
            {
                *static_cast<uint8_t*>(p) = v;
            }

            static uint8_t swap(uint8_t v)
            {
                return v;
            }
        };

        template <>
        struct endian<byte_order::native_endian, uint8_t>
        {
            static uint8_t read(const void* p)
            {
                return *static_cast<const uint8_t*>(p);
            }

            static void write(void* p, uint8_t v)
            {
                *static_cast<uint8_t*>(p) = v;
            }

            static uint8_t swap(uint8_t v)
            {
                return v;
            }
        };

#if !STDEXT_LITTLE_ENDIAN
        template <>
        struct endian<byte_order::little_endian, uint16_t>
        {
            static uint16_t read(const void* p)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap16(*static_cast<const uint16_t*>(p));
#else
#error Implement me
#endif
            }

            static void write(void* p, uint16_t v)
            {
#if STDEXT_COMPILER_GCC
                *static_cast<uint16_t*>(p) = __builtin_bswap16(v);
#else
#error Implement me
#endif
            }

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
            static uint32_t read(const void* p)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap32(*static_cast<const uint32_t*>(p));
#else
#error Implement me
#endif
            }

            static void write(void* p, uint32_t v)
            {
#if STDEXT_COMPILER_GCC
                *static_cast<uint32_t*>(p) = __builtin_bswap32(v);
#else
#error Implement me
#endif
            }

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
            static uint64_t read(const void* p)
            {
#if STDEXT_COMPILER_GCC
                return __builtin_bswap32(*static_cast<const uint64_t*>(p));
#else
#error Implement me
#endif
            }

            static void write(void* p, uint64_t v)
            {
#if STDEXT_COMPILER_GCC
                *static_cast<uint64_t*>(p) = __builtin_bswap64(v);
#else
#error Implement me
#endif
            }

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
            static uint16_t read(const void* p)
            {
#if STDEXT_ARCH_X86
                return _load_be_u16(p);
#elif STDEXT_COMPILER_GCC
                return __builtin_bswap16(*static_cast<const uint16_t*>(p));
#else
#error Implement me
#endif
            }

            static void write(void* p, uint16_t v)
            {
#if STDEXT_ARCH_X86
                _store_be_u16(p, v);
#elif STDEXT_COMPILER_GCC
                *static_cast<uint16_t*>(p) = __builtin_bswap16(v);
#else
#error Implement me
#endif
            }

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
            static uint32_t read(const void* p)
            {
#ifdef STDEXT_ARCH_X86
                return _load_be_u32(p);
#elif STDEXT_COMPILER_GCC
                return __builtin_bswap32(*static_cast<const uint32_t*>(p));
#else
#error Implement me
#endif
            }

            static void write(void* p, uint32_t v)
            {
#if STDEXT_ARCH_X86
                _store_be_u32(p, v);
#elif STDEXT_COMPILER_GCC
                *static_cast<uint32_t*>(p) = __builtin_bswap32(v);
#else
#error Implement me
#endif
            }

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
            static uint64_t read(const void* p)
            {
#ifdef STDEXT_ARCH_X86
                return _load_be_u64(p);
#elif STDEXT_COMPILER_GCC
                return __builtin_bswap64(*static_cast<const uint64_t*>(p));
#else
#error Implement me
#endif
            }

            static void write(void* p, uint64_t v)
            {
#if STDEXT_ARCH_X86
                _store_be_u64(p, v);
#elif STDEXT_COMPILER_GCC
                *static_cast<uint64_t*>(p) = __builtin_bswap64(v);
#else
#error Implement me
#endif
            }

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
            uint16_t read(const void* p)
            {
                return endian<byte_order::little_endian, uint16_t>::read(p);
            }

            static void write(void* p, uint16_t v)
            {
                endian<byte_order::little_endian, uint16_t>::write(p, v);
            }

            static uint16_t swap(uint16_t v)
            {
                return endian<byte_order::little_endian, uint16_t>::swap(v);
            }
        };

        template <>
        struct endian<byte_order::pdp_endian, uint32_t>
        {
            static uint32_t read(const void* p)
            {
                auto pb = static_cast<const uint8_t*>(p);
                return (uint32_t(endian<byte_order::little_endian, uint16_t>::read(pb)) << 16)
                    | endian<byte_order::little_endian, uint16_t>::read(pb + 2);
            }

            static void write(void* p, uint32_t v)
            {
                auto pb = static_cast<uint8_t*>(p);
                endian<byte_order::little_endian, uint16_t>::write(pb, uint16_t(v >> 16));
                endian<byte_order::little_endian, uint16_t>::write(pb + 2, uint16_t(v));
            }

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
            static uint64_t read(const void* p)
            {
                auto pb = static_cast<const uint8_t*>(p);
                return (uint64_t(endian<byte_order::pdp_endian, uint32_t>::read(pb)) << 32)
                    | endian<byte_order::pdp_endian, uint32_t>::read(pb + 4);
            }

            static void write(void* p, uint64_t v)
            {
                auto pb = static_cast<uint8_t*>(p);
                endian<byte_order::pdp_endian, uint32_t>::write(pb, uint32_t(v >> 32));
                endian<byte_order::pdp_endian, uint32_t>::write(pb + 4, uint32_t(v));
            }

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
