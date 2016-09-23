//
//  helpers.hpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 19.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#include "defs.h"
#ifndef helpers_hpp
#define helpers_hpp

template<class T>
std::string toString(const T& t, bool *ok = NULL)
{
    std::ostringstream stream;
    stream << t;
    if(ok != NULL)
        *ok = (stream.fail() == false);
    return stream.str();
}

bool isPowerOfTwo(dim_t n);

#endif /* helpers_hpp */