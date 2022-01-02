//
//  multi.h
//  stdext
//
//  Created by James Touton on 9/05/16.
//  Copyright © 2016 James Touton. All rights reserved.
//

#ifndef STDEXT_MULTI_INCLUDED
#define STDEXT_MULTI_INCLUDED
#pragma once

#include <stdext/meta.h>
#include <stdext/utility.h>

#include <tuple>

#include <cassert>


namespace stdext
{
    template <typename... Interfaces> class multi_ref;
    template <typename... Interfaces> class multi_ptr;

    namespace _private
    {
        template <typename To>
        struct is_convertible_to
        {
            template <typename From>
            using templ = std::is_convertible<From, To>;
        };

        template <typename From>
        struct is_convertible_from
        {
            template <typename To>
            using templ = std::is_convertible<From, To>;
        };

        template <typename... From> struct is_convertible_from_one_of;
        template <typename From0, typename... From>
        struct is_convertible_from_one_of<From0, From...>
        {
            template <typename To>
            struct templ
            {
                static constexpr auto _count = is_convertible_from_one_of<From...>::template templ<To>::_count + (std::is_convertible_v<From0, To> ? 1 : 0);
                static constexpr auto value = _count == 1;
            };
        };
        template <>
        struct is_convertible_from_one_of<>
        {
            template <typename To>
            struct templ
            {
                static constexpr size_t _count = 0;
                static constexpr auto value = false;
            };
        };

        template <typename U>
        struct is_same_as
        {
            template <typename T>
            using templ = std::is_same<T, U>;
        };
    }

    template <typename... Interfaces>
    class multi_ref
    {
    public:
        // References are copyable, but not assignable.
        constexpr multi_ref(const multi_ref&) = default;
        multi_ref& operator = (const multi_ref&) = delete;

        template <typename T, STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces&...>, _private::is_convertible_from<T&>::template templ>)>
        constexpr multi_ref(T& obj) noexcept : _interfaces(static_cast<Interfaces*>(addressof(obj))...)
        {
        }

        template <typename... OtherInterfaces,
            STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces&...>, _private::is_convertible_from_one_of<OtherInterfaces&...>::template templ>)>
        constexpr multi_ref(const multi_ref<OtherInterfaces...>& ref) noexcept
            : _interfaces(addressof(ref.template as<std::remove_reference_t<list_find_first_fulfilling_t<type_list<OtherInterfaces&...>, _private::is_convertible_to<Interfaces&>::template templ>>>())...)
        {
        }

        template <size_t Count = sizeof...(Interfaces), STDEXT_REQUIRES(Count > 1)>
        explicit constexpr multi_ref(Interfaces&... refs) noexcept : _interfaces(addressof(refs)...)
        {
        }

    public:
        constexpr multi_ptr<Interfaces...> operator & () const noexcept
        {
            return multi_ptr<Interfaces...>(std::get<Interfaces*>(_interfaces)...);
        }

        constexpr multi_ref& rebind(const multi_ref& obj)
        {
            _interfaces = obj._interfaces;
            return *this;
        }

        template <typename T, STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces&...>, _private::is_convertible_from<T&>::template templ>)>
        constexpr multi_ref& rebind(T& obj) noexcept
        {
            _interfaces = std::make_tuple(static_cast<Interfaces*>(addressof(obj))...);
            return *this;
        }

        template <typename... OtherInterfaces,
            STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces&...>, _private::is_convertible_from_one_of<OtherInterfaces&...>::template templ>)>
        constexpr multi_ref& rebind(const multi_ref<OtherInterfaces...>& ref) noexcept
        {
            _interfaces = std::make_tuple(addressof(ref.template as<std::remove_reference_t<list_find_first_fulfilling_t<type_list<OtherInterfaces&...>, _private::is_convertible_to<Interfaces&>::template templ>>>())...);
            return *this;
        }

        template <typename Interface, STDEXT_REQUIRES(list_any_of_v<type_list<Interfaces...>, _private::is_same_as<Interface>::template templ>)>
        constexpr Interface& as() const noexcept
        {
            return *std::get<Interface*>(_interfaces);
        }

        template <typename Interface, STDEXT_REQUIRES(list_any_of_v<type_list<Interfaces...>, _private::is_same_as<Interface>::template templ>)>
        constexpr const Interface& as_const() const noexcept
        {
            return *std::get<Interface*>(_interfaces);
        }

    private:
        std::tuple<Interfaces*...> _interfaces;
    };

    template <typename... Interfaces>
    class multi_ptr
    {
    public:
        constexpr multi_ptr() = default;

        constexpr multi_ptr(std::nullptr_t) noexcept : _interfaces(static_cast<Interfaces*>(nullptr)...)
        {
        }

        template <typename T, STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from<T*>::template templ>)>
        constexpr multi_ptr(T* p) noexcept : _interfaces(static_cast<Interfaces*>(p)...)
        {
        }

        template <typename... OtherInterfaces,
            STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from_one_of<OtherInterfaces*...>::template templ>)>
        constexpr multi_ptr(const multi_ptr<OtherInterfaces...>& ptr) noexcept
            : _interfaces(ptr.template as<std::remove_pointer_t<list_find_first_fulfilling_t<type_list<OtherInterfaces*...>, _private::is_convertible_to<Interfaces*>::template templ>>>()...)
        {
        }

        template <size_t Count = sizeof...(Interfaces), STDEXT_REQUIRES(Count > 1)>
        explicit constexpr multi_ptr(Interfaces*... ps) noexcept : _interfaces(ps...)
        {
            assert(((... && (ps == nullptr)) || (... && (ps != nullptr))));
        }

        constexpr multi_ptr& operator = (std::nullptr_t) noexcept
        {
            _interfaces = std::make_tuple(static_cast<Interfaces*>(nullptr)...);
            return *this;
        }

        template <typename T, STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from<T*>::template templ>)>
        constexpr multi_ptr& operator = (T* p) noexcept
        {
            _interfaces = std::make_tuple(static_cast<Interfaces*>(p)...);
            return *this;
        }

        template <typename... OtherInterfaces,
            STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from_one_of<OtherInterfaces*...>::template templ>)>
        constexpr multi_ptr& operator = (const multi_ptr<OtherInterfaces...>& ptr) noexcept
        {
            _interfaces = std::make_tuple(ptr.template as<std::remove_pointer_t<list_find_first_fulfilling_t<type_list<OtherInterfaces*...>, _private::is_convertible_to<Interfaces*>::template templ>>>()...);
            return *this;
        }

        friend constexpr bool operator == (const multi_ptr& a, const multi_ptr& b) noexcept { return a._interfaces == b._interfaces; }
        friend constexpr bool operator != (const multi_ptr& a, const multi_ptr& b) noexcept { return a._interfaces != b._interfaces; }

        friend constexpr bool operator == (const multi_ptr& a, nullptr_t) noexcept { return sizeof...(Interfaces) == 0 || std::get<0>(a._interfaces) == nullptr; }
        friend constexpr bool operator != (const multi_ptr& a, nullptr_t) noexcept { return sizeof...(Interfaces) != 0 && std::get<0>(a._interfaces) != nullptr; }
        friend constexpr bool operator == (nullptr_t, const multi_ptr& b) noexcept { return sizeof...(Interfaces) == 0 || std::get<0>(b._interfaces) == nullptr; }
        friend constexpr bool operator != (nullptr_t, const multi_ptr& b) noexcept { return sizeof...(Interfaces) != 0 && std::get<0>(b._interfaces) != nullptr; }

        template <typename T, STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from<T*>::template templ>)>
        friend constexpr bool operator == (const multi_ptr& a, T* b) noexcept { return (... && (std::get<Interfaces*>(a._interfaces) == b)); }
        template <typename T, STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from<T*>::template templ>)>
        friend constexpr bool operator != (const multi_ptr& a, T* b) noexcept { return (... || (std::get<Interfaces*>(a._interfaces) != b)); }
        template <typename T, STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from<T*>::template templ>)>
        friend constexpr bool operator == (T* a, const multi_ptr& b) noexcept { return (... && (a == std::get<Interfaces*>(b._interfaces))); }
        template <typename T, STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from<T*>::template templ>)>
        friend constexpr bool operator != (T* a, const multi_ptr& b) noexcept { return (... || (a != std::get<Interfaces*>(b._interfaces))); }

    public:
        constexpr multi_ref<Interfaces...> operator * () const noexcept
        {
            return multi_ref<Interfaces...>(*std::get<Interfaces*>(_interfaces)...);
        }

        template <typename Interface, STDEXT_REQUIRES(list_any_of_v<type_list<Interfaces...>, _private::is_same_as<Interface>::template templ>)>
        constexpr Interface* as() const noexcept
        {
            return std::get<Interface*>(_interfaces);
        }

        template <typename Interface, STDEXT_REQUIRES(list_any_of_v<type_list<Interfaces...>, _private::is_same_as<Interface>::template templ>)>
        constexpr const Interface* as_const() const noexcept
        {
            return std::get<Interface*>(_interfaces);
        }

    private:
        std::tuple<Interfaces*...> _interfaces;
    };
}

#endif
