#if defined _MSVC_TRADITIONAL && !_MSVC_TRADITIONAL
#pragma warning(disable: 5105)  // macro expansion producing 'defined' has undefined behavior
#endif

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
