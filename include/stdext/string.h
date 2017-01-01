//
//  string.h
//  stdext
//
//  Created by James Touton on 5/11/16.
//  Copyright © 2016 James Touton. All rights reserved.
//

#ifndef STDEXT_STRING_INCLUDED
#define STDEXT_STRING_INCLUDED
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
        return make_terminated_generator(forward<Iterator>(i),
            cstring_termination_predicate<iterator_value_type_t<::std::decay_t<Iterator>>>);
    }
}

#endif
