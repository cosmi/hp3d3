//
//  Node.h
//  HP-spline
//
//  Created by Marcin on 10.05.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef Node_h
#define Node_h
#include "Element.h"
#include <unordered_set>
#include <list>

template<int DIMS>
class Node {
    using Element = Element<DIMS>;
protected:
    std::unordered_set<Element*> support;
public:
    const std::unordered_set<Element*>& getSupport() const {
        return support;
    }
    virtual ~Node(){};
};


#endif /* Node_h */
