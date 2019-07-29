//
//  array_view.h
//  stdext
//
//  Created by James Touton on 10/25/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef STDEXT_ARRAY_VIEW_INCLUDED
#define STDEXT_ARRAY_VIEW_INCLUDED
#pragma once

#include <stdext/range.h>

#include <cstddef>


namespace stdext
{
    template <class T>
    class array_view : public delimited_iterator_range<T*>
    {
    public:
        using reference = typename delimited_iterator_range<T*>::reference;
        using const_iterator = const T*;
        using reverse_iterator = ::std::reverse_iterator<T*>;
        using const_reverse_iterator = ::std::reverse_iterator<const T*>;
        using const_reference = const T&;
        using pointer = T*;
        using size_type = size_t;

    public:
        constexpr array_view() noexcept = default;
        template <class U, size_t size,
            STDEXT_REQUIRES(std::is_same_v<std::remove_cv_t<T>, std::remove_cv_t<U>>
            && std::is_convertible_v<U*, T*>)>
        constexpr array_view(U (&a)[size]) noexcept : delimited_iterator_range<T*>(a, a + size) { }
        constexpr array_view(pointer p, size_type size) noexcept : delimited_iterator_range<T*>(p, p + size) { }

    public:
        template <class U, size_t size,
            STDEXT_REQUIRES(std::is_same_v<std::remove_cv_t<T>, std::remove_cv_t<U>>
            && std::is_convertible_v<U*, T*>)>
        constexpr array_view& operator = (U (&a)[size]) { begin_pos(a); end_pos(a + size); return *this; }
        constexpr auto data() const noexcept { return this->begin(); }
        constexpr bool empty() const noexcept { return this->begin() == this->end(); }

        constexpr reference front() noexcept { return *this->begin(); }
        constexpr const_reference front() const noexcept { return *this->begin(); }
        constexpr reference back() noexcept { return *std::prev(this->end()); }
        constexpr const_reference back() const noexcept { return *std::prev(this->end()); }
    };

    template <class T>
    class const_array_view : public delimited_iterator_range<const T*>
    {
    public:
        using const_iterator = const T*;
        using reverse_iterator = ::std::reverse_iterator<const T*>;
        using const_reverse_iterator = ::std::reverse_iterator<const T*>;
        using const_reference = const T&;
        using pointer = const T*;
        using size_type = size_t;

    public:
        constexpr const_array_view() noexcept = default;
        template <class U, size_t size,
            STDEXT_REQUIRES(std::is_same_v<std::remove_cv_t<T>, std::remove_cv_t<U>>
            && std::is_convertible_v<U*, const T*>)>
        constexpr const_array_view(U(&a)[size]) noexcept : delimited_iterator_range<const T*>(a, a + size) { }
        constexpr const_array_view(pointer p, size_type size) noexcept : delimited_iterator_range<const T*>(p, p + size) { }

    public:
        template <class U, size_t size,
            STDEXT_REQUIRES(std::is_same_v<std::remove_cv_t<T>, std::remove_cv_t<U>>
            && std::is_convertible_v<U*, const T*>)>
        constexpr const_array_view& operator = (U(&a)[size]) { this->begin_pos(a); this->end_pos(a + size); return *this; }
        constexpr auto data() const noexcept { return this->begin(); }
        constexpr bool empty() const noexcept { return this->begin() == this->end(); }

        constexpr const_reference front() const noexcept { return *this->begin(); }
        constexpr const_reference back() const noexcept { return *std::prev(this->end()); }
    };
}

#endif
