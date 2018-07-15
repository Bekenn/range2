//
//  filesystem.h
//  stdext
//
//  Created by James Touton on 12/24/16.
//  Copyright © 2016 James Touton. All rights reserved.
//

#ifndef STDEXT_FILESYSTEM_INCLUDED
#define STDEXT_FILESYSTEM_INCLUDED
#pragma once

#include <stdext/string_view.h>

#include <locale>
#include <string>


// This file defines types and functions based on a subset of C++17's filesystem library
// dealing with file system paths.  These definitions are provided here so that they may
// be used on systems predating C++17's standard library.

namespace stdext
{
    // From file.h; should probably only exist in this file.
#ifdef _WIN32
    using path_char = wchar_t;
    constexpr path_char directory_separator = L'\\';
#define _PATH_STR(s) L ## s
#define PATH_STR(s) _PATH_STR(s)
#else
    using path_char = char;
    constexpr path_char directory_separator = '/';
#define PATH_STR(s) s
#endif
}

namespace stdext { namespace filesystem
{
    namespace detail
    {
        bool is_directory_separator(path_char c)
        {
            return c == PATH_STR('/') || c == directory_separator;
        }
    }

    // paths
    class path
    {
    public:
        using value_type = path_char;
        using string_type = ::std::basic_string<value_type>;
        using string_view_type = basic_string_view<value_type>; // extension
        static constexpr value_type preferred_separator = directory_separator;

    public:
        path() noexcept = default;
        path(string_type&& source) : pathstring(move(source)) { }
        template <class Source> path(const Source& source);
        template <class InputIterator> path(InputIterator first, InputIterator last);
        template <class Source> path(const Source& source, const ::std::locale& loc);
        template <class InputIterator> path(InputIterator first, InputIterator last, const ::std::locale& loc);

    public:
        path& operator = (string_type&& source)
        {
            return assign(source);
        }

        path& assign(string_type&& source)
        {
            pathstring = move(source);
            return *this;
        }

        template <class Source> path& operator = (const Source& source);
        template <class Source> path& assign(const Source& source);
        template <class InputIterator> path& assign(InputIterator first, InputIterator& last);

        path& operator /= (const path& p)
        {
            if (p.empty())
                return *this;

            if (has_relative_path()
                && !detail::is_directory_separator(pathstring.back())
                && !detail::is_directory_separator(p.native().front()))
            {
                pathstring.push_back(preferred_separator);
            }

            pathstring.append(p.native());
            return *this;
        }

        template <class Source> path& operator /= (const Source& source);
        template <class Source> path& append(const Source& source);
        template <class InputIterator> path& append(InputIterator first, InputIterator last);

        path& operator += (const path& x)
        {
            return *this += string_view_type(x.native());
        }

        path& operator += (const string_type& x)
        {
            return *this += string_view_type(x);
        }

        path& operator += (string_view_type x)
        {
            pathstring.append(x.begin(), x.end());
            return *this;
        }

        path& operator += (const value_type* x)
        {
            return *this += string_view_type(x);
        }

        path& operator += (value_type x)
        {
            pathstring.push_back(x);
            return *this;
        }

        template <class Source> path& operator += (const Source& x);
        template <class EcharT> path& operator += (EcharT x);
        template <class Source> path& concat(const Source& x);
        template <class InputIterator> path& concat(InputIterator first, InputIterator last);

        void clear() noexcept
        {
            pathstring.clear();
        }

        path& make_preferred()
        {
            if (preferred_separator == '/')
                return *this;

            replace(make_range(pathstring.begin(), pathstring.end()), PATH_STR('/'), preferred_separator);
            return *this;
        }

        path& remove_filename()
        {
        }

        path& replace_filename(const path& replacement);
        path& replace_extension(const path& replacement = path());
        void swap(path& rhs) noexcept;

        const string_type& native() const noexcept;
        const value_type* c_str() const noexcept;
        operator string_type () const;

        template <class EcharT, class traits = ::std::char_traits<EcharT>, class Allocator = ::std::allocator<EcharT>>
        ::std::basic_string<EcharT, traits, Allocator> string(const Allocator& a = Allocator()) const;
        ::std::string string() const;
        ::std::wstring wstring() const;
        ::std::string u8string() const;
        ::std::u16string u16string() const;
        ::std::u32string u32string() const;

        template <class EcharT, class traits = ::std::char_traits<EcharT>, class Allocator = ::std::allocator<EcharT>>
        ::std::basic_string<EcharT, traits, Allocator> generic_string(const Allocator& a = Allocator()) const;
        ::std::string generic_string() const;
        ::std::wstring generic_wstring() const;
        ::std::string generic_u8string() const;
        ::std::u16string generic_u16string() const;
        ::std::u32string generic_u32string() const;

        int compare(const path& p) const noexcept;
        int compare(const string_type& s) const;
        int compare(string_view_type s) const;
        int compare(const value_type* s) const;

        path root_name() const;
        path root_directory() const;
        path root_path() const;
        path relative_path() const;
        path parent_path() const;
        path filename() const;
        path stem() const;
        path extension() const;

        bool empty() const noexcept;
        bool has_root_name() const;
        bool has_root_directory() const;
        bool has_root_path() const;
        bool has_relative_path() const;
        bool has_parent_path() const;
        bool has_filename() const;
        bool has_stem() const;
        bool has_extension() const;
        bool is_absolute() const;
        bool is_relative() const;

        path lexically_normal() const;
        path lexically_relative(const path& base) const;
        path lexically_proximate(const path& base) const;

        class iterator;
        using const_iterator = iterator;

        iterator begin() const;
        iterator end() const;

    private:
        string_type pathstring;
    };

    class path::iterator
    {
    public:
        using iterator_category = ::std::bidirectional_iterator_tag;
        using value_type = path;
        using difference_type = ptrdiff_t;
        using pointer = const path*;
        using reference = const path&;

    public:
        iterator() noexcept : location() { }
    private:
        explicit iterator(path::string_type::const_iterator p);

    public:
        friend bool operator == (const iterator& a, const iterator& b) noexcept
        {
            return a.location == b.location;
        }
        friend bool operator != (const iterator& a, const iterator& b) noexcept
        {
            return !(a == b);
        }

    public:
        reference operator * () noexcept
        {
            return component;
        }

        pointer operator -> () noexcept
        {
            return &component;
        }

        iterator& operator ++ () noexcept;
        iterator& operator -- () noexcept;

        iterator operator ++ (int) noexcept
        {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

        iterator operator -- (int) noexcept
        {
            iterator tmp = *this;
            --*this;
            return tmp;
        }

    private:
        path component; // lame; this should be a view type
        path::string_type::const_iterator location;
    };

    // path non-member functions
    void swap(path& lhs, path& rhs) noexcept;
    size_t hash_value(const path& p) noexcept;

    bool operator == (const path& lhs, const path& rhs) noexcept;
    bool operator != (const path& lhs, const path& rhs) noexcept;
    bool operator <  (const path& lhs, const path& rhs) noexcept;
    bool operator <= (const path& lhs, const path& rhs) noexcept;
    bool operator >  (const path& lhs, const path& rhs) noexcept;
    bool operator >= (const path& lhs, const path& rhs) noexcept;

    path operator / (const path& lhs, const path& rhs);

    // path factory functions
    template <class Source> path u8path(const Source& source);
    template <class InputIterator> path u8path(InputIterator first, InputIterator last);

    // filesystem errors
    class filesystem_error;

    // filesystem operations
    path current_path();
    path temp_directory_path();

    bool equivalent(const path& p1, const path& p2);

    path absolute(const path& p, const path& base = current_path());
    path canonical(const path& p, const path& base = current_path());
    path proximate(const path& p, const path& base = current_path());
    path relative(const path& p, const path& base = current_path());
    path system_complete(const path& p);
    path weakly_canonical(const path& p);
} }

#endif
