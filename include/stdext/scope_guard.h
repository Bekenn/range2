//
//  utility.h
//  stdext
//
//  Created by James Touton on 2/20/21.
//  Copyright © 2021 James Touton. All rights reserved.
//

#ifndef STDEXT_SCOPE_GUARD_INCLUDED
#define STDEXT_SCOPE_GUARD_INCLUDED
#pragma once

#include <stdext/utility.h>

#include <exception>
#include <type_traits>


namespace stdext
{
    template <class Callback>
    class scope_guard
    {
    public:
        explicit scope_guard(const Callback& cb) : _cb(cb), _valid(true) { }
        explicit scope_guard(Callback&& cb) : _cb(move(cb)), _valid(true) { }

        scope_guard(const scope_guard&) = delete;
        scope_guard& operator = (const scope_guard&) = delete;

        scope_guard(scope_guard&& other)
            : _cb(move(other._cb)), _valid(exchange(other._valid, false))
        {
        }

        scope_guard& operator = (scope_guard&& other)
        {
            _cb = move(other._cb);
            _valid = exchange(other._valid, false);
        }

        ~scope_guard() noexcept(noexcept(_cb()))
        {
            if (_valid)
                _cb();
        }

    public:
        void invalidate() noexcept
        {
            _valid = false;
        }

    private:
        Callback _cb;
        bool _valid;
    };

    template <class Callback>
    class scope_error_guard : public scope_guard<Callback>
    {
    public:
        explicit scope_error_guard(const Callback& cb, bool call_on_error)
            : scope_guard<Callback>(cb), _exception_count(std::uncaught_exceptions()), _call_on_error(call_on_error)
        {
        }

        explicit scope_error_guard(Callback&& cb, bool call_on_error)
            : scope_guard<Callback>(move(cb)), _exception_count(std::uncaught_exceptions()), _call_on_error(call_on_error)
        {
        }

        scope_error_guard(scope_error_guard&&) = default;
        scope_error_guard& operator = (scope_error_guard&&) = default;

        ~scope_error_guard() noexcept(noexcept(std::declval<Callback>()()))
        {
            bool error = std::uncaught_exceptions() != _exception_count;
            if (_call_on_error != error)
                this->invalidate();
        }

    private:
        int _exception_count;
        bool _call_on_error;
    };

    template <class Callback>
    auto make_scope_guard(Callback&& cb)
    {
        return scope_guard<std::decay_t<Callback>>(forward<Callback>(cb));
    }

    template <class Callback>
    auto make_scope_error_guard(Callback&& cb, bool call_on_error)
    {
        return scope_error_guard<std::decay_t<Callback>>(forward<Callback>(cb), call_on_error);
    }

#define at_scope_exit(...) auto STDEXT_CONCAT(scope_guard_, __COUNTER__) = ::stdext::make_scope_guard(__VA_ARGS__)
#define at_scope_success(...) auto STDEXT_CONCAT(scope_success_, __COUNTER__) = ::stdext::make_scope_error_guard(__VA_ARGS__, false)
#define at_scope_error(...) auto STDEXT_CONCAT(scope_error_, __COUNTER__) = ::stdext::make_scope_error_guard(__VA_ARGS__, true)
}

#endif
