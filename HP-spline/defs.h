//
//  types.h
//  HP3d
//
//  Created by Marcin on 05.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef HP3d_types_h
#define HP3d_types_h

#include <sstream>

using dim_t = long long int;
using result_t = long long int;
using mask_t = long long unsigned;
#define FOR(x, n) for(typeof(n) x = 0, __n = (n); x < __n; x++)
#define FORI(x, a, n) for(typeof(n) x = (a), __n = (n); x < __n; x++)
#define FORR(x, n) for(typeof(n) x = (n)-1; x >= 0; x--)

#include "helpers.hpp"


#endif
