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


template<int DIMS, class Projection = IdentityProjection>
std::pair<SVGPoint, SVGPoint> calculateBounds(const CellId<DIMS>& cid, const Projection& p) {
    auto points = cid.getLowerDimensionalityBounds(0);
    bool first = true;
    SVGPoint ptMin(0,0), ptMax(0,0);
    for(auto & point: points) {
        if(first) {
            ptMin = ptMax = p(point.getFrom());
            first = false;
        } else {
            auto pt2 = p(point.getFrom());
            std::cout << "PT>" << pt2.x << " " << pt2.y << std::endl;
            if(pt2.x < ptMin.x) ptMin.x = pt2.x;
            if(pt2.y < ptMin.y) ptMin.y = pt2.y;
            if(pt2.x > ptMax.x) ptMax.x = pt2.x;
            if(pt2.y > ptMax.y) ptMax.y = pt2.y;
        }
    }
    return std::make_pair(ptMin, ptMax);
}

template<int DIMS, class Projection = IdentityProjection, class SVGCanvas = SVGCanvas<> >
class SVGRenderer {
    SVGCanvas canvas;
    Projection projection;
public:
    SVGRenderer(const char* s, const CellId<DIMS>& bounds, const Projection& p):
        canvas(s, calculateBounds(bounds, p).first, calculateBounds(bounds, p).second),
        projection(p) {
    }
    SVGRenderer(const char* s, const CellId<DIMS>& bounds):
        projection(),
        canvas(s, calculateBounds(bounds, Projection()).first, calculateBounds(bounds, Projection()).second){
    }
    void drawCellDiagonal(const CellId<DIMS>& cid) {
        canvas.drawLine(projection(cid.getFrom()), projection(cid.getTo()));
    }
    void drawBounds(const CellId<DIMS>& bounds) {
        auto lines = bounds.getLowerDimensionalityBounds(1);
        for(auto & line: lines) {
            drawCellDiagonal(line);
        }
    }
    void close() {
        canvas.close();
    }
    void getCanvas() {
        return canvas;
    }
};


#endif /* SVGRenderer_hpp */
