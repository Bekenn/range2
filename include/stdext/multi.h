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

#include <tuple>


namespace stdext
{
    template <class... Interfaces>
    class multi
    {
    private:
        template <template <class...> class Trait, class... Args>
        struct value_trait_binder
        {
            template <class... Ts>
            struct templ
            {
                static constexpr auto value = Trait<Args..., Ts...>::value;
            };
        };

    public:
        template <class T, REQUIRES(list_all_of_v<type_list<Interfaces...>, value_trait_binder<::std::is_base_of, T>::templ>)>
        multi(T& obj) : interfaces(static_cast<Interfaces&>(obj)...)
        {
        }

    public:
        template <class Interface, REQUIRES(list_any_of_v<type_list<Interfaces...>, value_trait_binder<::std::is_same, Interface>::templ>)>
        Interface& as() noexcept
        {
            return ::std::get<Interface&>(interfaces);
        }

        template <class Interface, REQUIRES(list_any_of_v<type_list<Interfaces...>, value_trait_binder<::std::is_same, Interface>::templ>)>
        const Interface& as() const noexcept
        {
            return ::std::get<Interface&>(interfaces);
        }

    private:
        ::std::tuple<Interfaces&...> interfaces;
    };
}

#endif
