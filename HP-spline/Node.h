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
#include "GeometryHelpers.hpp"
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
    virtual double getValue(const Coordinate<DIMS>& x) const = 0;
    double getValue(const Point<DIMS>& x) const {
        Coordinate<DIMS> v[DIMS];
        FOR(i, DIMS) v[i] = x[i];
        return getValue(v);
    }
//    double getValue(const std::initializer_list<double>& x) {
//        double v[DIMS];
//        int i = 0;
//        for(double x1: x) {
//            v[i++] = x1;
//        }
//        return getValue(v);
//    }
    virtual CellId<DIMS> getAnchor() const = 0;
    virtual CellId<DIMS> getBounds() const = 0;
};


#endif /* Node_h */
