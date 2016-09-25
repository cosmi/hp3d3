//
//  GeometryHelpers.hpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 25.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef GeometryHelpers_hpp
#define GeometryHelpers_hpp

#include <array>
#include "CellId.h"

template <int DIMS>
using Coordinate = std::array<double, DIMS>;

template <int DIMS>
inline Coordinate<DIMS> getMidPoint(const CellId<DIMS>& cid) {
    Coordinate<DIMS> ret;
    FOR(i, DIMS) {
        ret[i] = (cid.getFrom()[i] + cid.getTo()[i])/2.;
    }
    return ret;
}

#endif /* GeometryHelpers_hpp */
