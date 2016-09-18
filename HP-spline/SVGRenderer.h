//
//  SVGRenderer.hpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 18.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef SVGRenderer_hpp
#define SVGRenderer_hpp

#include "SVGCanvas.h"
#include "projections.hpp"
#include "CellId.h"
#include "Point.h"

template<int DIMS, class Projection = IdentityProjection, class SVGCanvas = SVGCanvas<> >
class SVGRenderer {
    SVGCanvas canvas;
    Projection projection;
public:
    SVGRenderer(const std::string& s, const CellId<DIMS>& bounds, const Projection& p):
        canvas(s, p(bounds.getFrom()), p(bounds.getTo())),
        projection(p) {
    }
    SVGRenderer(const std::string& s, const CellId<DIMS>& bounds):
        canvas(s, bounds.getFrom()[0], bounds.getFrom()[1], bounds.getTo()[0], bounds.getTo()[1]){
    }
    void drawCellDiagonal(const CellId<DIMS>& cid) {
        canvas.drawLine(p(cid.getFrom()), p(cid.getTo()));
    }
    void drawBounds(const CellId<DIMS>& bounds) {
        auto lines = bounds.getLowerDimensionalityBounds(1);
        for(auto & line: lines) {
            drawCellDiagonal(line);
        }
    }
};


#endif /* SVGRenderer_hpp */
