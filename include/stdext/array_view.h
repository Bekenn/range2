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
    template <typename T>
    class array_view : public delimited_iterator_range<T*>
    {
    public:
        using reference = typename delimited_iterator_range<T*>::reference;
        using const_iterator = const T*;
        using reverse_iterator = std::reverse_iterator<T*>;
        using pointer = T*;
        using size_type = size_t;

    public:
        constexpr array_view() noexcept = default;
        template <typename U, STDEXT_REQUIRES(std::is_convertible_v<U (*)[], T (*)[]>)>
        constexpr array_view(const array_view<U>& other) noexcept : delimited_iterator_range<T*>(other.begin(), other.end()) { }
        template <typename U, size_t Size, STDEXT_REQUIRES(std::is_convertible_v<U (*)[], T (*)[]>)>
        constexpr array_view(U (&a)[Size]) noexcept : delimited_iterator_range<T*>(a, a + Size) { }
        template <typename U, STDEXT_REQUIRES(std::is_convertible_v<U (*)[], T (*)[]>)>
        constexpr array_view(U* p, size_type Size) noexcept : delimited_iterator_range<T*>(p, p + Size) { }

    public:
        template <typename U, STDEXT_REQUIRES(std::is_convertible_v<U (*)[], T (*)[]>)>
        constexpr array_view& operator = (const array_view<U>& other) noexcept { this->begin_pos(other.begin()); this->end_pos(other.end()); return *this; }
        template <typename U, size_t Size, STDEXT_REQUIRES(std::is_convertible_v<U (*)[], T (*)[]>)>
        constexpr array_view& operator = (U (&a)[Size]) { this->begin_pos(a); this->end_pos(a + Size); return *this; }
        constexpr auto data() const noexcept { return this->begin(); }
        constexpr bool empty() const noexcept { return this->begin() == this->end(); }

        constexpr reference front() const noexcept { return *this->begin(); }
        constexpr reference back() const noexcept { return *std::prev(this->end()); }
    };
}

#endif
