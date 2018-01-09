//
//  string.h
//  stdext
//
//  Created by James Touton on 5/11/16.
//  Copyright © 2016 James Touton. All rights reserved.
//

#ifndef STDEXT_STRING_INCLUDED
#define STDEXT_STRING_INCLUDED
#pragma once

#include <stdext/generator.h>
#include <stdext/string_view.h>


namespace stdext
{
    template <class charT, class traits = std::char_traits<charT>, class Allocator = std::allocator<charT>>
    class basic_stringbuf;
    using stringbuf = basic_stringbuf<char>;
    using wstringbuf = basic_stringbuf<wchar_t>;
    using u16stringbuf = basic_stringbuf<char16_t>;
    using u32stringbuf = basic_stringbuf<char32_t>;

    template <class charT, class traits, class Allocator>
    class basic_stringbuf
    {
    public:
        using string_type = std::basic_string<charT, traits, Allocator>;
        using string_view_type = basic_string_view<charT, traits>;

    public:
        bool operator () (charT value)
        {
            str.push_back(value);
            return true;
        }

        bool operator () (string_view_type value)
        {
            str.append(value.data(), value.size());
            return true;
        }

        string_type extract()
        {
            return std::exchange(str, { });
        }

        void reset()
        {
            str.clear();
        }

    private:
        string_type str;
    };

    template <class Char>
    bool cstring_termination_predicate(const Char& value)
    {
        return value == Char();
    }

    template <class Iterator, REQUIRES(is_iterator<::std::decay_t<Iterator>>::value)>
    auto make_cstring_generator(Iterator&& i)
    {
        return make_terminated_generator(forward<Iterator>(i),
            cstring_termination_predicate<iterator_value_type_t<::std::decay_t<Iterator>>>);
    }
}

#endif
