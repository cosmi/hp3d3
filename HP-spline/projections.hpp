//
//  projections.hpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 18.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef projections_hpp
#define projections_hpp

#include "SVGHelpers.hpp"
#include "Point.h"


struct ObliqueProjection {
    double zx, zy;
    ObliqueProjection(double zx=0.35, double zy=0.25): zx(zx), zy(zy) {}
    
    SVGPoint operator()(const Point<3>& p) const {
        return SVGPoint(p[0] + p[2]*zx, p[1]+p[2]*zy);
    }
};


struct IdentityProjection {
    SVGPoint operator()(const Point<2>& p) const {
        return SVGPoint(p[0], p[1]);
    }
};


#endif /* projections_hpp */
