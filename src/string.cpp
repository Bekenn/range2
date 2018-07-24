//
//  string.cpp
//  stdext
//
//  Created by James Touton on 7/24/18.
//  Copyright © 2018 James Touton. All rights reserved.
//

#include <stdext/string.h>

#include <stdexcept>

#include <cwchar>


namespace stdext
{
    std::string to_mbstring(const wchar_t* str)
    {
        std::mbstate_t state = {};
        auto length = std::wcsrtombs(nullptr, &str, 0, &state);
        if (length == size_t(-1))
            throw std::runtime_error("Character encoding conversion error");

        std::string result(length, '\0');
        std::wcsrtombs(result.data(), &str, length, &state);
        return result;
    }

    std::wstring to_wstring(const char* str)
    {
        std::mbstate_t state = {};
        auto length = std::mbsrtowcs(nullptr, &str, 0, &state);
        if (length == size_t(-1))
            throw std::runtime_error("Character encoding conversion error");

        std::wstring result(length, '\0');
        std::mbsrtowcs(result.data(), &str, length, &state);
        return result;
    }
}
