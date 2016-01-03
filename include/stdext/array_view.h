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

#include "range.h"

#include <cstddef>


namespace stdext
{
    template <class T>
    class array_view : public delimited_iterator_range<T*>
    {
    public:
        using const_reference = const T&;
        using typename delimited_iterator_range<T*>::pointer;
        using size_type = ::std::size_t;

    public:
        array_view() noexcept = default;
        array_view(pointer p, size_type size) noexcept : delimited_iterator_range<T*>(p, p + size) { }

    public:
        auto data() const noexcept { return this->first; }
    };

    template <class T>
    class const_array_view : public delimited_iterator_range<const T*>
    {
    public:
        using typename delimited_iterator_range<const T*>::pointer;
        using size_type = ::std::size_t;

    public:
        const_array_view() noexcept = default;
        const_array_view(pointer p, size_type size) noexcept : delimited_iterator_range<const T*>(p, p + size) { }

    public:
        auto data() const noexcept { return this->first; }
    };
}

#endif
