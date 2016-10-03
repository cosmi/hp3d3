//
//  SVGHelpers.hpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 18.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef SVGHelpers_hpp
#define SVGHelpers_hpp

struct SVGPoint {
    double x, y;
    SVGPoint(double x, double y): x(x), y(y) {}
    
    SVGPoint operator+ (const SVGPoint& p) const {
        return SVGPoint(x + p.x, y + p.y);
    }
};

struct SVGLine {
    SVGPoint p1, p2;
    SVGLine(double x1, double y1, double x2, double y2):p1(x1, y1), p2(x2, y2) {}
};

#endif /* SVGHelpers_hpp */
