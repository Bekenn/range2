//
//  concept.h
//  iolib
//
//  Created by James Touton on 10/18/15.
//  Copyright © 2015 James Touton. All rights reserved.
//

#ifndef IOLIB_CONCEPT_INCLUDED
#define IOLIB_CONCEPT_INCLUDED
#pragma once

#include <type_traits>

#define REQUIRED(...) ::std::enable_if_t<(__VA_ARGS__), ::std::nullptr_t>
#define REQUIRES(...) REQUIRED(__VA_ARGS__) = nullptr

#endif
