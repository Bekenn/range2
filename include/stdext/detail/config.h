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
#define STDEXT_COMPILER_CLANG 0
#define STDEXT_COMPILER_GCC 0
#define STDEXT_COMPILER_MSVC 0

#ifdef __clang__
#undef STDEXT_COMPILER_CLANG
#define STDEXT_COMPILER_CLANG 1
#endif
#ifdef __GNUC__
#undef STDEXT_COMPILER_GCC
#define STDEXT_COMPILER_GCC 1
#endif
#ifdef _MSC_VER
#undef STDEXT_COMPILER_MSVC
#define STDEXT_COMPILER_MSVC 1
#endif

// Processor detection
#define STDEXT_ARCH_X86 0
#define STDEXT_ARCH_X86_32 0
#define STDEXT_ARCH_X86_64 0
#define STDEXT_ARCH_ITANIUM 0
#define STDEXT_ARCH_ARM 0
#define STDEXT_ARCH_ARM_THUMB 0
#define STDEXT_ARCH_ARM64 0
#define STDEXT_ARCH_POWERPC 0
#define STDEXT_ARCH_POWERPC64 0

#if defined __i386__ || defined _M_IX86
#undef STDEXT_ARCH_X86
#undef STDEXT_ARCH_X86_32
#define STDEXT_ARCH_X86 1
#define STDEXT_ARCH_X86_32 1
#endif

#if defined __x86_64__ || defined _M_AMD64
#undef STDEXT_ARCH_X86
#undef STDEXT_ARCH_X86_64
#define STDEXT_ARCH_X86 1
#define STDEXT_ARCH_X86_64 1
#endif

#if defined __ia64__ || defined _M_IA64
#undef STDEXT_ARCH_ITANIUM
#define STDEXT_ARCH_ITANIUM 1
#endif

#if defined __arm__ || defined _M_ARM
#undef STDEXT_ARCH_ARM
#define STDEXT_ARCH_ARM 1
#if defined __thumb__ || defined _M_ARMT
#undef STDEXT_ARCH_ARM_THUMB
#define STDEXT_ARCH_ARM_THUMB 1
#endif
#endif

#ifdef __aarch64__
#undef STDEXT_ARCH_ARM64
#define STDEXT_ARCH_ARM64 1
#endif

#if defined __ppc__ || defined _M_PPC
#undef STDEXT_ARCH_POWERPC
#define STDEXT_ARCH_POWERPC 1
#endif

#ifdef __ppc64__
#undef STDEXT_ARCH_POWERPC64
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

// Platform detection
#define STDEXT_PLATFORM_WINDOWS 0
#define STDEXT_PLATFORM_MAC 0
#define STDEXT_PLATFORM_LINUX 0

#ifdef _WIN32
#undef STDEXT_PLATFORM_WINDOWS
#define STDEXT_PLATFORM_WINDOWS 1
#endif

#ifdef __APPLE__
#undef STDEXT_PLATFORM_MAC
#define STDEXT_PLATFORM_MAC 1
#endif

#ifdef __linux__
#undef STDEXT_PLATFORM_LINUX
#define STDEXT_PLATFORM_LINUX 1
#endif

// Feature detection
#ifndef __has_include
#define __has_include(x) 0
#endif

#define STDEXT_HAS_C_UNICODE 0

#if __has_include(<cuchar>)
#undef STDEXT_HAS_C_UNICODE
#define STDEXT_HAS_C_UNICODE 1
#endif

#endif
