//
//  helpers.hpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 19.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#include "defs.h"
#include <iostream>
#include <ctime>
#ifndef helpers_hpp
#define helpers_hpp

inline void clockTick(const std::string& s) {
    std::cout << "CLOCK " << clock() << ":"  << s << std::endl;
}

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


template<class ptrClass>
struct less_by_ptr{
    bool operator()(const ptrClass& a, const ptrClass& b) const {
        if(!a) return b;
        if(!b) return false;
        return *a < *b;
    }
};



#endif /* helpers_hpp */
