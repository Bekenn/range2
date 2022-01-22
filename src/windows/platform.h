#ifndef STDEXT_IMPL_PLATFORM_INCLUDED
#define STDEXT_IMPL_PLATFORM_INCLUDED
#pragma once

#if defined _MSVC_TRADITIONAL && !_MSVC_TRADITIONAL
#pragma warning(push)
#pragma warning(disable: 5105)  // macro expansion producing 'defined' has undefined behavior
#endif

#define NOMINMAX
#include <Windows.h>

#if defined _MSVC_TRADITIONAL && !_MSVC_TRADITIONAL
#pragma warning(pop)
#endif

#endif
