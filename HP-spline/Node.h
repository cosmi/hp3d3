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


class Node {
    std::unordered_set<Element*> support;
public:
    list<Element*> getSupport() const {
        return support;
    }
    virtual ~Node() = 0;
};


#endif /* Node_h */
