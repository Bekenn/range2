#ifndef STDEXT_SPAN_INCLUDED
#define STDEXT_SPAN_INCLUDED
#pragma once

#include <stdext/iterator.h>
#include <stdext/traits.h>
#include <stdext/types.h>

#include <cassert>


namespace stdext
{
    // class template span
    template <typename ElementType>
    class span;

    namespace _private
    {
        template <typename T> constexpr bool is_span = false;
        template <typename T> constexpr bool is_span<span<T>> = true;
    }

    template <typename ElementType>
    class span
    {
    public:
        // constants and types
        using element_type = ElementType;
        using value_type = std::remove_cv_t<ElementType>;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using pointer = element_type*;
        using const_pointer = const element_type*;
        using reference = element_type&;
        using const_reference = const element_type&;
        using iterator = element_type*;
        using reverse_iterator = std::reverse_iterator<iterator>;

        // constructors, copy, and assignment
        constexpr span() noexcept = default;
        constexpr span(pointer first, size_type count) noexcept : _data(first), _size(count) { }
        constexpr span(pointer first, pointer last) noexcept : _data(first), _size(last - first) { }
        template <size_t N>
        constexpr span(element_type (&arr)[N]) noexcept : _data(arr), _size(N) { }
        template <typename R, typename D = std::remove_cv_t<R>, typename U = std::remove_pointer_t<decltype(std::data(declval<R&>()))>, typename = decltype(std::size(declval<R&>())),
            STDEXT_REQUIRES(!_private::is_span<D> && !std::is_array_v<D> && std::is_convertible_v<U (*)[], element_type (*)[]>)>
        constexpr span(R& r) : _data(std::data(r)), _size(std::size(r)) { }
        constexpr span(const span& other) noexcept = default;
        template <typename OtherElementType, STDEXT_REQUIRES(std::is_convertible_v<OtherElementType (*)[], element_type (*)[]>)>
        constexpr span(const span<OtherElementType>& s) noexcept : _data(s.data()), _size(s.size()) { }

        ~span() noexcept = default;

        constexpr span& operator=(const span& other) noexcept = default;

        // subviews
        constexpr span<element_type> first(size_type count) const { assert(count <= _size); return { _data, count }; }
        constexpr span<element_type> last(size_type count) const { assert(count <= _size); return { _data + _size - count, _size - count }; }
        constexpr span<element_type> subspan(size_type offset) const { assert(offset <= _size); return { _data + offset, _size - offset }; }
        constexpr span<element_type> subspan(size_type offset, size_type count) const { assert(offset <= _size); assert(count <= _size - offset); return { _data + offset, count }; }

        // observers
        constexpr size_type size() const noexcept { return _size; }
        [[nodiscard]] constexpr bool empty() const noexcept { return _size == 0; }

        // element access
        constexpr reference operator[](size_type idx) const { assert(idx < _size); return _data[idx]; }
        constexpr reference front() const { assert(_size != 0); return _data[0]; }
        constexpr reference back() const { assert(_size != 0); return _data[_size - 1]; }
        constexpr pointer data() const noexcept { return _data; }

        // iterator support
        constexpr iterator begin() const noexcept { return _data; }
        constexpr iterator end() const noexcept { return _data + _size; }
        constexpr reverse_iterator rbegin() const noexcept { return reverse_iterator(_data + _size); }
        constexpr reverse_iterator rend() const noexcept { return reverse_iterator(_data); }

    private:
        pointer _data = nullptr;
        size_type _size = 0;
    };

    // views of object representation
    template <typename ElementType>
    span<const byte> as_bytes(span<ElementType> s) noexcept
    {
        return { reinterpret_cast<const byte*>(s.data()), sizeof(ElementType) * s.size() };
    }

    template <typename ElementType, STDEXT_REQUIRES(!std::is_const_v<ElementType>)>
    span<byte> as_writable_bytes(span<ElementType> s) noexcept
    {
        return { reinterpret_cast<byte*>(s.data()), sizeof(ElementType) * s.size() };
    }
}

#endif
