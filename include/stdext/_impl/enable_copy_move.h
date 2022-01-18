#ifndef STDEXT_IMPL_ENABLE_COPY_MOVE_INCLUDED
#define STDEXT_IMPL_ENABLE_COPY_MOVE_INCLUDED
#pragma once

#include <stdext/_impl/config.h>


namespace stdext::_private
{
    template <bool> struct enable_copy_construct { };
    template <> struct enable_copy_construct<false>
    {
        enable_copy_construct() = default;
        enable_copy_construct(const enable_copy_construct&) = delete;
        enable_copy_construct(enable_copy_construct&&) = default;
        enable_copy_construct& operator = (const enable_copy_construct&) = default;
        enable_copy_construct& operator = (enable_copy_construct&&) = default;
    };

    template <bool> struct enable_move_construct { };
    template <> struct enable_move_construct<false>
    {
        enable_move_construct() = default;
        enable_move_construct(const enable_move_construct&) = default;
        enable_move_construct(enable_move_construct&&) = delete;
        enable_move_construct& operator = (const enable_move_construct&) = default;
        enable_move_construct& operator = (enable_move_construct&&) = default;
    };

    template <bool> struct enable_copy_assign { };
    template <> struct enable_copy_assign<false>
    {
        enable_copy_assign() = default;
        enable_copy_assign(const enable_copy_assign&) = default;
        enable_copy_assign(enable_copy_assign&&) = default;
        enable_copy_assign& operator = (const enable_copy_assign&) = delete;
        enable_copy_assign& operator = (enable_copy_assign&&) = default;
    };

    template <bool> struct enable_move_assign { };
    template <> struct enable_move_assign<false>
    {
        enable_move_assign() = default;
        enable_move_assign(const enable_move_assign&) = default;
        enable_move_assign(enable_move_assign&&) = default;
        enable_move_assign& operator = (const enable_move_assign&) = default;
        enable_move_assign& operator = (enable_move_assign&&) = delete;
    };
}

#endif
