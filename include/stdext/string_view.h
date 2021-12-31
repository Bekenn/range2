//
//  string_view.h
//  stdext
//
//  Created by James Touton on 10/25/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_STRING_VIEW_INCLUDED
#define STDEXT_STRING_VIEW_INCLUDED
#pragma once

#include <stdext/algorithm.h>
#include <stdext/array_view.h>

#include <algorithm>
#include <string>


// This string_view is similar to the proposed string_view in Library Fundamentals, but has
// been adapted to fit the definition of a stdext range.  One particularly notable difference
// is that the equality operator returns true only for views that point to the same memory;
// that is, two string_views are equal iff their iterators are equal.  This provides more
// consistent behavior and performance between assignment and comparison.

namespace stdext
{
    template <typename charT, typename traits = std::char_traits<std::remove_cv_t<charT>>>
    class basic_string_view;

    using string_view = basic_string_view<char>;
    using const_string_view = basic_string_view<const char>;
    using wstring_view = basic_string_view<wchar_t>;
    using const_wstring_view = basic_string_view<const wchar_t>;
    using u16string_view = basic_string_view<char16_t>;
    using const_u16string_view = basic_string_view<const char16_t>;
    using u32string_view = basic_string_view<char32_t>;
    using const_u32string_view = basic_string_view<const char32_t>;

    int stoi(string_view str, size_t* idx = nullptr, int base = 10);
    long stol(string_view str, size_t* idx = nullptr, int base = 10);
    unsigned long stoul(string_view str, size_t* idx = nullptr, int base = 10);
    long long stoll(string_view str, size_t* idx = nullptr, int base = 10);
    unsigned long long stoull(string_view str, size_t* idx = nullptr, int base = 10);
    float stof(string_view str, size_t* idx = nullptr);
    double stod(string_view str, size_t* idx = nullptr);
    long double stold(string_view str, size_t* idx = nullptr);

    template <typename charT, typename traits>
    class basic_string_view : public array_view<const charT>
    {
    public:
        using value_type = typename array_view<const charT>::value_type;
        using pointer = typename array_view<const charT>::pointer;
        using reverse_iterator = typename array_view<const charT>::reverse_iterator;
        using size_type = typename array_view<const charT>::size_type;
        using traits_type = traits;
        template <typename Allocator = std::allocator<std::remove_cv_t<value_type>>>
        using string_type = std::basic_string<std::remove_cv_t<value_type>, traits_type, Allocator>;
        using const_pointer = const value_type*;

        static constexpr auto npos = size_type(-1);

    public:
        using array_view<const charT>::array_view;

        constexpr basic_string_view() noexcept = default;

        constexpr basic_string_view(const basic_string_view& str, size_type pos, size_type n = npos) noexcept
            : array_view<const charT>(str.data() + pos, std::min(n, str.length()) - pos) { }

        template <typename Allocator>
        basic_string_view(const string_type<Allocator>& str) noexcept
            : array_view<const charT>(str.data(), str.length()) { }

        constexpr basic_string_view(pointer s, size_type n) noexcept
            : array_view<const charT>(s, n) { }

        constexpr basic_string_view(pointer s) noexcept
            : array_view<const charT>(s, traits_type::length(s)) { }

        constexpr basic_string_view(pointer begin, pointer end) noexcept
            : array_view<const charT>(begin, std::distance(begin, end)) { }

        template <typename Allocator>
        auto& operator = (const string_type<Allocator>& str) noexcept
        {
            auto p = str.data();
            this->begin_pos(str.data());
            this->end_pos(p + str.length());
            return *this;
        }

        constexpr auto& operator = (pointer s) noexcept
        {
            this->begin_pos(s);
            this->end_pos(s + traits_type::length(s));
            return *this;
        }

    public:
        constexpr auto length() const noexcept { return this->size(); }

        constexpr void remove_prefix(size_t n)
        {
            drop_first(*this, n);
        }

        constexpr void remove_suffix(size_t n)
        {
            drop_last(*this, n);
        }

        constexpr auto find(basic_string_view str, size_type pos = 0)
        {
            pos = std::min(pos, length());
            auto i = search(subrange_from(*this, this->begin() + pos), str);
            return i == this->end() ? npos : size_type(std::distance(this->begin(), i));
        }

        constexpr auto find(const_pointer s, size_type pos, size_type n)
        {
            return find(basic_string_view(s, n), pos);
        }

        constexpr auto find(value_type c, size_type pos = 0)
        {
            return find(basic_string_view(std::addressof(c), 1), pos);
        }

        constexpr auto rfind(basic_string_view str, size_type pos = npos)
        {
            pos = std::min(pos, length());
            pos = std::min(pos + str.length(), length());
            auto i = find_end(subrange_from(*this, this->begin() + pos), str);
            return i == this->end() ? npos : size_type(std::distance(this->begin(), i));
        }

        constexpr auto rfind(const_pointer s, size_type pos, size_type n)
        {
            return rfind(basic_string_view(s, n), pos);
        }

        constexpr auto rfind(value_type c, size_type pos = npos)
        {
            return rfind(basic_string_view(std::addressof(c), 1), pos);
        }

        constexpr auto find_first_of(basic_string_view str, size_type pos = 0)
        {
            pos = std::min(pos, length());
            auto i = find_first_of(subrange_from(*this, this->begin() + pos), str);
            return i == this->end() ? npos : size_type(std::distance(this->begin(), i));
        }

        constexpr auto find_first_of(const_pointer s, size_type pos, size_type n)
        {
            return find_first_of(basic_string_view(s, n), pos);
        }

        constexpr auto find_first_of(value_type c, size_t pos = 0)
        {
            return find_first_of(basic_string_view(std::addressof(c), 1), pos);
        }

        constexpr auto find_last_of(basic_string_view str, size_type pos = 0)
        {
            pos = std::min(pos, length());
            auto reverse = make_reverse_range(subrange_to(*this, this->begin() + pos));
            auto i = reverse.base_pos(find_first_of(reverse, str));
            return i == this->begin() ? npos : size_type(std::distance(this->begin(), --i));
        }

        constexpr auto find_last_of(const_pointer s, size_type pos, size_type n)
        {
            return find_first_of(basic_string_view(s, n), pos);
        }

        constexpr auto find_last_of(value_type c, size_t pos = 0)
        {
            return find_first_of(basic_string_view(std::addressof(c), 1), pos);
        }

        constexpr auto find_first_not_of(basic_string_view str, size_type pos = 0)
        {
            pos = std::min(pos, length());
            auto i = find_if(subrange_from(*this, this->begin() + pos),
                [str](value_type c) { return str.find(c) == npos; });
            return i == this->end() ? npos : size_type(std::distance(this->begin(), i));
        }

        constexpr auto find_first_not_of(const_pointer s, size_type pos, size_type n)
        {
            return find_first_not_of(basic_string_view(s, n), pos);
        }

        constexpr auto find_first_not_of(value_type c, size_t pos = 0)
        {
            return find_first_not_of(basic_string_view(std::addressof(c), 1), pos);
        }

        constexpr auto find_last_not_of(basic_string_view str, size_type pos = 0)
        {
            pos = std::min(pos, length());
            auto reverse = make_reverse_range(subrange_to(*this, this->begin() + pos));
            auto i = reverse.base_pos(find_if(reverse,
                [str](value_type c) { return str.find(c) == npos; }));
            return i == this->begin() ? npos : size_type(std::distance(this->begin(), --i));
        }

        constexpr auto find_last_not_of(const_pointer s, size_type pos, size_type n)
        {
            return find_first_of(basic_string_view(s, n), pos);
        }

        constexpr auto find_last_not_of(value_type c, size_t pos = 0)
        {
            return find_first_of(basic_string_view(std::addressof(c), 1), pos);
        }

        constexpr auto substr(size_type pos = 0, size_type n = npos)
        {
            if (pos > length())
                throw std::out_of_range("string index out of range");
            n = std::min(n, length() - pos);
            return basic_string_view(this->data() + pos, n);
        }

        constexpr auto compare(basic_string_view str)
        {
            auto rlen = std::min(length(), str.length());
            auto result = traits_type::compare(this->data(), str.data(), rlen);
            return result == 0
                ? length() < str.length()
                    ? -1
                    : length() == str.length() ? 0 : 1
                : result;
        }

        constexpr auto compare(size_type pos1, size_type n1, basic_string_view str)
        {
            n1 = std::min(n1, length() - pos1);
            return basic_string_view(*this, pos1, n1).compare(str);
        }

        constexpr auto compare(const_pointer s)
        {
            return compare(basic_string_view(s));
        }

        constexpr auto compare(size_type pos, size_type n1, const_pointer s)
        {
            return basic_string_view(*this, pos, n1).compare(s);
        }

        constexpr auto compare(size_type pos, size_type n1, const_pointer s, size_type n2)
        {
            return basic_string_view(*this, pos, n1).compare(basic_string_view(s, n2));
        }
    };
}

#endif
