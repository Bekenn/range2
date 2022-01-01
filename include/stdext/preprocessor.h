#ifndef STDEXT_PREPROCESSOR_INCLUDED
#define STDEXT_PREPROCESSOR_INCLUDED
#pragma once

#include <stdext/_impl/config.h>


// token pasting
#define STDEXT_PP_PASTE(a, b) xSTDEXT_PP_PASTE(a, b)
#define xSTDEXT_PP_PASTE(a, b) a ## b

// stringification
#define STDEXT_PP_STRINGIFY(x) #x

// remove enclosing parentheses (if present)
#define STDEXT_PP_DEPAREN(x) xSTDEXT_PP_DEPAREN_CLEANUP(xSTDEXT_PP_DEPAREN x)
#define xSTDEXT_PP_DEPAREN(...) xSTDEXT_PP_DEPAREN __VA_ARGS__
#define xSTDEXT_PP_DEPAREN_CLEANUP(...) xSTDEXT_PP_DEPAREN_CLEANUP_(__VA_ARGS__)
#define xSTDEXT_PP_DEPAREN_CLEANUP_(...) xSTDEXT_PP_NO_ ## __VA_ARGS__
#define xSTDEXT_PP_NO_xSTDEXT_PP_DEPAREN

#endif
