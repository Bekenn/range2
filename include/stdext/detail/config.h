//
//  config.h
//  stdext
//
//  Created by James Touton on 5/15/16.
//  Copyright © 2016 James Touton. All rights reserved.
//

#ifndef STDEXT_CONFIG_INCLUDED
#define STDEXT_CONFIG_INCLUDED
#pragma once

// Compiler detection.  Note that these are NOT mutually exclusive
// due to emulation modes supported by some compilers!
#ifdef __clang__
#define STDEXT_COMPILER_CLANG 1
#endif
#ifdef __GNUC__
#define STDEXT_COMPILER_GCC 1
#endif
#ifdef _MSC_VER
#define STDEXT_COMPILER_MSVC 1
#endif

// Processor detection
#if defined __i386__ || defined _M_IX86
#define STDEXT_ARCH_X86 1
#define STDEXT_ARCH_X86_32 1
#endif

#if defined __x86_64__ || defined _M_AMD64
#define STDEXT_ARCH_X86 1
#define STDEXT_ARCH_X86_64 1
#endif

#if defined __ia64__ || defined _M_IA64
#define STDEXT_ARCH_ITANIUM 1
#endif

#if defined __arm__ || defined _M_ARM
#define STDEXT_ARCH_ARM 1
#if defined __thumb__ || defined _M_ARMT
#define STDEXT_ARCH_ARM_THUMB 1
#endif
#endif

#ifdef __aarch64__
#define STDEXT_ARCH_ARM64 1
#endif

#if defined __ppc__ || defined _M_PPC
#define STDEXT_ARCH_POWERPC 1
#endif

#ifdef __ppc64__
#define STDEXT_ARCH_POWERPC64 1
#endif

// Endianness detection
#define STDEXT_LITTLE_ENDIAN 0
#define STDEXT_BIG_ENDIAN 0
#define STDEXT_PDP_ENDIAN 0

#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#undef STDEXT_LITTLE_ENDIAN
#define STDEXT_LITTLE_ENDIAN 1
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#undef STDEXT_BIG_ENDIAN
#define STDEXT_BIG_ENDIAN 1
#elif __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__
#undef STDEXT_PDP_ENDIAN
#define STDEXT_PDP_ENDIAN 1
#else
#error Unknown endianness
#endif
#elif STDEXT_COMPILER_MSVC
#if defined _M_AMD64 || defined _M_ARM || defined _M_ARM64 || defined _M_IX86
#undef STDEXT_LITTLE_ENDIAN
#define STDEXT_LITTLE_ENDIAN 1
#else
#error Unknown architecture
#endif
#else
#error Unknown compiler
#endif

#if _MSC_VER >= 1900
#ifndef __cpp_constexpr
#define __cpp_constexpr 200704
#endif
#endif

#if __cpp_constexpr < 201304
#define CONSTEXPR14
#else
#define CONSTEXPR14 constexpr
#endif

#endif
