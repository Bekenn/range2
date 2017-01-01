//
//  config.h
//  stdext
//
//  Created by James Touton on 5/15/16.
//  Copyright © 2016 James Touton. All rights reserved.
//

#ifndef STDEXT_CONFIG_INCLUDED
#define STDEXT_CONFIG_INCLUDED
#pragma once

#if _MSC_VER >= 1900
#ifndef __cpp_constexpr
#define __cpp_constexpr 200704
#endif
#endif

#if __cpp_constexpr < 201304
#define CONSTEXPR14
#else
#define CONSTEXPR14 constexpr
#endif

#endif
