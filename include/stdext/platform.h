//
//  generator.h
//  stdext
//
//  Created by James Touton on 7/21/18.
//  Copyright � 2018 James Touton. All rights reserved.
//

#ifndef STDEXT_PLATFORM_INCLUDED
#define STDEXT_PLATFORM_INCLUDED
#pragma once

#include <stdext/_impl/config.h>


namespace stdext
{
#if STDEXT_PLATFORM_WINDOWS
    using path_char = wchar_t;
    constexpr path_char directory_separator = L'\\';
#define PATH_STR_(s) L ## s
#define PATH_STR(s) PATH_STR_(s)
#else
    using path_char = char;
    constexpr path_char directory_separator = '/';
#define PATH_STR(s) s
#endif
}

#endif
