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
#include <stdext/utility.h>

#include <string>
#include <system_error>

#include <cerrno>
#include <climits>
#include <cwchar>

#if STDEXT_HAS_C_UNICODE
#include <cuchar>
#endif

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
            return stdext::exchange(str, { });
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

    template <class Iterator, STDEXT_REQUIRES(is_iterator<std::decay_t<Iterator>>::value)>
    auto make_cstring_generator(Iterator&& i)
    {
        return make_terminated_generator(stdext::forward<Iterator>(i),
            cstring_termination_predicate<iterator_value_type_t<std::decay_t<Iterator>>>);
    }

    // Conversion between narrow and wide execution character sets
    std::string to_mbstring(const wchar_t* str);
    std::wstring to_wstring(const char* str);

    namespace _private
    {
#if STDEXT_HAS_C_UNICODE
        inline size_t to_mb(char* s, char16_t c16, std::mbstate_t* ps)
        {
            return std::c16rtomb(s, c16, ps);
        }

        inline size_t to_mb(char* s, char32_t c32, std::mbstate_t* ps)
        {
            return std::c32rtomb(s, c32, ps);
        }
#endif

        inline size_t to_mb(char* s, wchar_t wc, std::mbstate_t* ps)
        {
            return std::wcrtomb(s, wc, ps);
        }
    }

    template <class Generator>
    class to_multibyte_generator
    {
    public:
        using iterator_category = generator_tag;
        using value_type = char;
        using difference_type = ptrdiff_t;
        using pointer = const char*;
        using reference = const char&;
        using generator = Generator;

    public:
        to_multibyte_generator() = default;
        to_multibyte_generator(const generator& g) : _g(g) { next(); }
        to_multibyte_generator(generator&& g) : _g(stdext::move(g)) { next(); }

    public:
        friend bool operator == (const to_multibyte_generator& a, const to_multibyte_generator& b) noexcept
        {
            return a._g == b._g
                && a._state == b._state
                && a._current == b._current
                && std::equal(a._value + a._current, a._value + std::size(a._value), b._value);
        }

        friend bool operator != (const to_multibyte_generator& a, const to_multibyte_generator& b) noexcept
        {
            return !(a == b);
        }

        friend void swap(to_multibyte_generator& a, to_multibyte_generator& b)
        {
            swap(a._g, b._g);
            swap(a._state, b._state);
            swap(a._current, b._current);
            swap(a._value, b._value);
        }

    public:
        reference operator * () const noexcept
        {
            return _value[_current];
        }

        pointer operator -> () const noexcept
        {
            return &_value[_current];
        }

        to_multibyte_generator& operator ++ ()
        {
            next();
            return *this;
        }

        iterator_proxy<to_multibyte_generator> operator ++ (int)
        {
            iterator_proxy<to_multibyte_generator> proxy = _value;
            next();
            return proxy;
        }

        explicit operator bool() const noexcept
        {
            return _g || _current == std::size(_value);
        }

    private:
        void next()
        {
            if (_current == std::size(_value))
            {
                assert(bool(_g));
                auto length = _private::to_mb(_value, *_g, &_state);
                ++_g;
                if (length == size_t(-1))
                    throw std::system_error(errno, std::generic_category());

                _current = std::size(_value) - length;
                std::move(_value, _value + length, _value + _current);
                return;
            }

            ++_current;
        }

    private:
        generator _g = { };
        std::mbstate_t _state = { };
        size_t _current = std::size(_value);
        value_type _value[MB_LEN_MAX] = { };
    };

    template <class Generator>
    class to_wchar_generator
    {
    public:
        using iterator_category = generator_tag;
        using value_type = wchar_t;
        using difference_type = ptrdiff_t;
        using pointer = const wchar_t*;
        using reference = const wchar_t&;
        using generator = Generator;

    public:
        to_wchar_generator() noexcept : _g(), _state(), _value() { }
        to_wchar_generator(const generator& g) : _g(g), _state(), _value() { next(); }
        to_wchar_generator(generator&& g) : _g(stdext::move(g)), _state(), _value() { next(); }

    public:
        friend bool operator == (const to_wchar_generator& a, const to_wchar_generator& b) noexcept
        {
            return a._g == b._g
                && a._state == b._state
                && a._value == b._value;
        }

        friend bool operator != (const to_wchar_generator& a, const to_wchar_generator& b) noexcept
        {
            return !(a == b);
        }

        friend void swap(to_wchar_generator& a, to_wchar_generator& b)
        {
            swap(a._g, b._g);
            swap(a._state, b._state);
            swap(a._value, b._value);
        }

    public:
        reference operator * () const noexcept
        {
            return _value;
        }

        pointer operator -> () const noexcept
        {
            return &_value;
        }

        to_wchar_generator& operator ++ ()
        {
            next();
            return *this;
        }

        iterator_proxy<to_wchar_generator> operator ++ (int)
        {
            iterator_proxy<to_wchar_generator> proxy = _value;
            next();
            return proxy;
        }

        explicit operator bool() const noexcept
        {
            return _g || !mbsinit(&_state);
        }

    private:
        void next()
        {
            size_t result;
            do
            {
                assert(bool(_g));
                result = std::mbrtowc(&_value, &*_g, 1, &_state);
                ++_g;
                if (result == size_t(-1))
                    throw std::system_error(errno, std::generic_category());
            } while (result == size_t(-2));
        }

    private:
        generator _g;
        mbstate_t _state;
        value_type _value;
    };

    struct to_multibyte_tag { };
    struct to_wchar_tag { };

    inline to_multibyte_tag to_multibyte() { return { }; }
    inline to_wchar_tag to_wchar() { return { }; }

    template <class Generator, STDEXT_REQUIRES(can_generate<std::decay_t<Generator>>::value)>
    auto operator >> (Generator&& g, to_multibyte_tag)
    {
        return to_multibyte_generator<generator_type_t<std::decay_t<Generator>, can_generate>>(as_generator(stdext::forward<Generator>(g)));
    }

    template <class Generator, STDEXT_REQUIRES(can_generate<std::decay_t<Generator>>::value)>
    auto operator >> (Generator&& g, to_wchar_tag)
    {
        return to_wchar_generator<generator_type_t<std::decay_t<Generator>, can_generate>>(as_generator(stdext::forward<Generator>(g)));
    }
}

#endif
