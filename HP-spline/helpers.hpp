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

inline bool isZero(double x) {
    return x < 10e-20 && x > -10e-20;
}

template<class Type>
Type abs(const Type& a) {
    if(a<0) return -a;
    return a;
}



#endif /* helpers_hpp */
