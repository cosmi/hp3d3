//
//  helpers.cpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 19.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#include "helpers.hpp"

bool isPowerOfTwo(dim_t n) {
    while(n && (n&1) == 0) n>>=1;
    return n == 1;
}
