//
//  string_view.h
//  iolib
//
//  Created by James Touton on 10/25/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef IOLIB_STRING_VIEW_INCLUDED
#define IOLIB_STRING_VIEW_INCLUDED
#pragma once

#include "algorithm.h"
#include "array_view.h"

#include <string>


namespace iolib
{
    template <class charT, class traits = ::std::char_traits<::std::remove_cv_t<charT>>>
    class basic_string_view;

    using string_view = basic_string_view<char>;
    using const_string_view = basic_string_view<const char>;
    using wstring_view = basic_string_view<wchar_t>;
    using const_wstring_view = basic_string_view<const wchar_t>;
    using u16string_view = basic_string_view<char16_t>;
    using const_u16string_view = basic_string_view<const char16_t>;
    using u32string_view = basic_string_view<char32_t>;
    using const_u32string_view = basic_string_view<const char32_t>;

    template <class charT, class traits>
    class basic_string_view : public array_view<charT>
    {
        using array_view<charT>::first;
        using array_view<charT>::last;

    public:
        using value_type = typename array_view<charT>::value_type;
        using pointer = typename array_view<charT>::pointer;
        using reverse_iterator = typename array_view<charT>::reverse_iterator;
        using size_type = typename array_view<charT>::size_type;
        using traits_type = traits;
        using string_type = ::std::basic_string<::std::remove_const_t<value_type>, traits_type>;
        using const_pointer = const value_type*;

        static constexpr auto npos = size_type(-1);

    public:
        using array_view<charT>::array_view;

        basic_string_view(const basic_string_view& str, size_type pos, size_type n = npos) noexcept
            : array_view<charT>(str.data() + pos, ::std::min(n, str.length()) - pos) { }

        template <class Allocator>
        basic_string_view(string_type& str) noexcept
            : array_view<charT>(const_cast<pointer>(str.data()), str.length()) { }

        template <class Allocator>
        basic_string_view(const string_type& str) noexcept
            : array_view<charT>(str.data(), str.length()) { }

        basic_string_view(pointer s, size_type n) noexcept
            : array_view<charT>(s, n) { }

        basic_string_view(pointer s) noexcept
            : array_view<charT>(s, traits_type::length(s)) { }

        basic_string_view(pointer begin, pointer end) noexcept
            : array_view<charT>(begin, ::std::distance(begin, end)) { }

        template <class Allocator>
        auto& operator = (string_type& str) noexcept
        {
            first = const_cast<pointer>(str.data());
            last = first + str.length();
            return *this;
        }

        template <class Allocator>
        auto& operator = (const string_type& str) noexcept
        {
            first = str.data();
            last = first + str.length();
            return *this;
        }

        auto& operator = (pointer s) noexcept
        {
            first = s;
            last = first + traits_type::length(s);
            return *this;
        }

    public:
        auto length() const noexcept { return this->size(); }

        auto find(basic_string_view str, size_type pos = 0)
        {
            pos = ::std::min(pos, length());
            auto i = ::std::search(first + pos, last, str.first, str.last);
            return i == last ? npos : size_type(distance(first, i));
        }

        auto find(const_pointer s, size_type pos, size_type n)
        {
            return find(basic_string_view(s, n), pos);
        }

        auto find(value_type c, size_type pos = 0)
        {
            return find(basic_string_view(::std::addressof(c), 1), pos);
        }

        auto rfind(basic_string_view str, size_type pos = npos)
        {
            pos = ::std::min(pos, length());
            pos = ::std::min(pos + str.length(), length());
            auto i = ::std::find_end(first + pos, last, str.first, str.last);
            return i == last ? npos : size_type(distance(first, i));
        }

        auto rfind(const_pointer s, size_type pos, size_type n)
        {
            return rfind(basic_string_view(s, n), pos);
        }

        auto rfind(value_type c, size_type pos = npos)
        {
            return rfind(basic_string_view(::std::addressof(c), 1), pos);
        }

        auto find_first_of(basic_string_view str, size_type pos = 0)
        {
            pos = ::std::min(pos, length());
            auto i = ::std::find_first_of(first + pos, last, str.begin(), str.end());
            return i == last ? npos : size_type(::std::distance(first, i));
        }

        auto find_first_of(const_pointer s, size_type pos, size_type n)
        {
            return find_first_of(basic_string_view(s, n), pos);
        }

        auto find_first_of(value_type c, size_t pos = 0)
        {
            return find_first_of(basic_string_view(::std::addressof(c), 1), pos);
        }

        auto find_last_of(basic_string_view str, size_type pos = 0)
        {
            pos = ::std::min(pos, length());
            auto i = ::std::find_first_of(reverse_iterator(first + pos), reverse_iterator(first), str.begin(), str.end());
            return i == last ? npos : size_type(::std::distance(first, i));
        }

        auto find_last_of(const_pointer s, size_type pos, size_type n)
        {
            return find_first_of(basic_string_view(s, n), pos);
        }

        auto find_last_of(value_type c, size_t pos = 0)
        {
            return find_first_of(basic_string_view(::std::addressof(c), 1), pos);
        }

        auto find_first_not_of(basic_string_view str, size_type pos = 0)
        {
            pos = ::std::min(pos, length());
            auto i = ::std::find_if(first + pos, last,
                [str](value_type c) { return str.find(c) == npos; });
            return i == last ? npos : size_type(::std::distance(first, i));
        }

        auto find_first_not_of(const_pointer s, size_type pos, size_type n)
        {
            return find_first_not_of(basic_string_view(s, n), pos);
        }

        auto find_first_not_of(value_type c, size_t pos = 0)
        {
            return find_first_not_of(basic_string_view(::std::addressof(c), 1), pos);
        }

        auto find_last_not_of(basic_string_view str, size_type pos = 0)
        {
            pos = ::std::min(pos, length());
            auto i = ::std::find_if(reverse_iterator(first + pos), reverse_iterator(first),
                [str](value_type c) { return str.find(c) == npos; });
            return i == last ? npos : size_type(::std::distance(first, i));
        }

        auto find_last_not_of(const_pointer s, size_type pos, size_type n)
        {
            return find_first_of(basic_string_view(s, n), pos);
        }

        auto find_last_not_of(value_type c, size_t pos = 0)
        {
            return find_first_of(basic_string_view(::std::addressof(c), 1), pos);
        }

        auto substr(size_type pos = 0, size_type n = npos)
        {
            if (pos > length())
                throw ::std::out_of_range("string index out of range");
            n = ::std::min(n, length());
            return basic_string_view(first + pos, n - pos);
        }

        auto compare(basic_string_view str)
        {
            auto rlen = ::std::min(length(), str.length());
            auto result = traits_type::compare(this->data(), str.data(), rlen);
            return result == 0
                ? length() < str.length()
                    ? -1
                    : length() == str.length() ? 0 : 1
                : result;
        }

        auto compare(size_type pos1, size_type n1, basic_string_view str)
        {
            n1 = ::std::min(n1, length() - pos1);
            return basic_string_view(*this, pos1, n1).compare(str);
        }

        auto compare(const_pointer s)
        {
            return compare(basic_string_view(s));
        }

        auto compare(size_type pos, size_type n1, const_pointer s)
        {
            return basic_string_view(*this, pos, n1).compare(s);
        }

        auto compare(size_type pos, size_type n1, const_pointer s, size_type n2)
        {
            return basic_string_view(*this, pos, n1).compare(basic_string_view(s, n2));
        }
    };
}

#endif
