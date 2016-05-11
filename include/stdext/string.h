#pragma once

#include "generator.h"


namespace stdext
{
    template <class Char>
    bool cstring_termination_predicate(const Char& value)
    {
        return value == Char();
    }

    template <class Iterator, REQUIRES(is_iterator<::std::decay_t<Iterator>>::value)>
    auto make_cstring_generator(Iterator&& i)
    {
        return make_terminated_generator(::std::forward<Iterator>(i),
            cstring_termination_predicate<value_type<::std::decay_t<Iterator>, is_iterator>>);
    }
}
