#include <stdext/platform.h>

#include <type_traits>


namespace test
{
#if STDEXT_PLATFORM_WINDOWS
    static_assert(std::is_same_v<stdext::path_char, wchar_t>);
    static_assert(stdext::directory_separator == L'\\');
#else
    static_assert(std::is_same_v<stdext::path_char, char>);
    static_assert(stdext::directory_separator == '/');
#endif

    static_assert(std::is_same_v<decltype(stdext::directory_separator), const stdext::path_char>);
    static_assert(std::is_same_v<std::decay_t<decltype(PATH_STR("filepath"))>, const stdext::path_char*>);
}
