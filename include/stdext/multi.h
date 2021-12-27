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
#include <stdext/traits.h>

#include <stdexcept>
#include <tuple>


namespace stdext
{
    namespace _private
    {
        template <class To>
        struct is_convertible_to
        {
            template <class From>
            using templ = std::is_convertible<From, To>;
        };

        template <class From>
        struct is_convertible_from
        {
            template <class To>
            using templ = std::is_convertible<From, To>;
        };

        template <class... From> struct is_convertible_from_one_of;
        template <class From0, class... From>
        struct is_convertible_from_one_of<From0, From...>
        {
            template <class To>
            struct templ
            {
                static constexpr auto _count = is_convertible_from_one_of<From...>::template templ<To>::_count + (std::is_convertible_v<From0, To> ? 1 : 0);
                static constexpr auto value = _count == 1;
            };
        };
        template <>
        struct is_convertible_from_one_of<>
        {
            template <class To>
            struct templ
            {
                static constexpr size_t _count = 0;
                static constexpr auto value = false;
            };
        };

        template <class U>
        struct is_same_as
        {
            template <class T>
            using templ = std::is_same<T, U>;
        };
    }

    template <class... Interfaces>
    class multi_ref
    {
    public:
        // References are copyable, but not assignable.
        constexpr multi_ref(const multi_ref&) = default;
        multi_ref& operator = (const multi_ref&) = delete;
        constexpr multi_ref(multi_ref&&) = default;
        multi_ref& operator = (multi_ref&&) = delete;

        template <class T, STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces&...>, _private::is_convertible_from<T&>::template templ>)>
        constexpr multi_ref(T& obj) noexcept : interfaces(static_cast<Interfaces&>(obj)...)
        {
        }

        template <class... OtherInterfaces,
            STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces&...>, _private::is_convertible_from_one_of<OtherInterfaces&...>::template templ>)>
        constexpr multi_ref(const multi_ref<OtherInterfaces...>& ref) noexcept
            : interfaces(ref.template as<std::remove_reference_t<list_find_first_fulfilling_t<type_list<OtherInterfaces&...>, _private::is_convertible_to<Interfaces&>::template templ>>>()...)
        {
        }

        template <size_t count = sizeof...(Interfaces), STDEXT_REQUIRES(count > 1)>
        explicit constexpr multi_ref(Interfaces&... refs) noexcept : interfaces(refs...)
        {
        }

    public:
        template <class Interface, STDEXT_REQUIRES(list_any_of_v<type_list<Interfaces...>, _private::is_same_as<Interface>::template templ>)>
        constexpr Interface& as() const noexcept
        {
            return std::get<Interface&>(interfaces);
        }

        template <class Interface, STDEXT_REQUIRES(list_any_of_v<type_list<Interfaces...>, _private::is_same_as<Interface>::template templ>)>
        constexpr const Interface& as_const() const noexcept
        {
            return std::get<Interface&>(interfaces);
        }

    private:
        std::tuple<Interfaces&...> interfaces;
    };

    template <class... Interfaces>
    class multi_ptr
    {
    public:
        constexpr multi_ptr() = default;

        template <class T, STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from<T*>::template templ>)>
        constexpr multi_ptr(T* p) noexcept : interfaces(static_cast<Interfaces*>(p)...)
        {
        }

        constexpr multi_ptr(std::nullptr_t) noexcept : interfaces(static_cast<Interfaces*>(nullptr)...)
        {
        }

        template <class... OtherInterfaces,
            STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from_one_of<OtherInterfaces*...>::template templ>)>
        constexpr multi_ptr(const multi_ptr<OtherInterfaces...>& ptr) noexcept
            : interfaces(ptr.template as<std::remove_pointer_t<list_find_first_fulfilling_t<type_list<OtherInterfaces*...>, _private::is_convertible_to<Interfaces*>::template templ>>>()...)
        {
        }

        template <class... OtherInterfaces,
            STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from_one_of<OtherInterfaces*...>::template templ>)>
        constexpr multi_ptr(const multi_ref<OtherInterfaces...>* ref) noexcept
            : interfaces(ref == nullptr ? nullptr : &ref->template as<std::remove_pointer_t<list_find_first_fulfilling_t<type_list<OtherInterfaces*...>, _private::is_convertible_to<Interfaces*>::template templ>>>()...)
        {
        }

        template <size_t count = sizeof...(Interfaces), STDEXT_REQUIRES(count > 1)>
        explicit constexpr multi_ptr(Interfaces*... ps) noexcept : interfaces(ps...)
        {
            if ((... || (ps == nullptr)) && !(... && (ps == nullptr)))
                throw std::invalid_argument("multi_ptr constructor args cannot mix null and non-null");
        }

    public:
        template <class T, STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from<T*>::template templ>)>
        constexpr multi_ptr& operator = (T* p) noexcept
        {
            interfaces = std::make_tuple(static_cast<Interfaces*>(p)...);
            return *this;
        }

        constexpr multi_ptr& operator = (std::nullptr_t) noexcept
        {
            interfaces = std::make_tuple(static_cast<Interfaces*>(nullptr)...);
            return *this;
        }

        template <class... OtherInterfaces,
            STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from_one_of<OtherInterfaces*...>::template templ>)>
        constexpr multi_ptr& operator = (const multi_ptr<OtherInterfaces...>& ptr) noexcept
        {
            interfaces = std::make_tuple(ptr.template as<std::remove_pointer_t<list_find_first_fulfilling_t<type_list<OtherInterfaces*...>, _private::is_convertible_to<Interfaces*>::template templ>>>()...);
            return *this;
        }

        template <class... OtherInterfaces,
            STDEXT_REQUIRES(list_all_of_v<type_list<Interfaces*...>, _private::is_convertible_from_one_of<OtherInterfaces*...>::template templ>)>
        constexpr multi_ptr& operator = (const multi_ref<OtherInterfaces...>* ref) noexcept
        {
            interfaces = std::make_tuple(ref == nullptr ? nullptr : &ref->template as<std::remove_pointer_t<list_find_first_fulfilling_t<type_list<OtherInterfaces*...>, _private::is_convertible_to<Interfaces*>::template templ>>>()...);
            return *this;
        }

        constexpr multi_ref<Interfaces...> operator * () const noexcept
        {
            return std::apply([](Interfaces*... ps)
            {
                return multi_ref<Interfaces...>(*ps...);
            }, interfaces);
        }

        template <class Interface, STDEXT_REQUIRES(list_any_of_v<type_list<Interfaces...>, _private::is_same_as<Interface>::template templ>)>
        constexpr Interface* as() const noexcept
        {
            return std::get<Interface*>(interfaces);
        }

        template <class Interface, STDEXT_REQUIRES(list_any_of_v<type_list<Interfaces...>, _private::is_same_as<Interface>::template templ>)>
        constexpr const Interface* as_const() const noexcept
        {
            return std::get<Interface*>(interfaces);
        }

    private:
        std::tuple<Interfaces*...> interfaces;
    };
}

#endif
