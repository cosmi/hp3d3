//
//  BSpline.cpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 24.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#include "BSpline.hpp"


double calcBSpline(double x, const std::vector<double>& knots, unsigned long degree, unsigned long offset) {
    if(degree == 0) {
        if(x < knots[offset] || x >= knots[offset+1]) return 0;
        return 1;
    } else {
        return (x-knots[offset])/(knots[offset+degree]-knots[offset])*calcBSpline(x,knots,degree-1, offset)
        + (knots[offset+degree+1]-x)/(knots[offset+degree+1]-knots[offset+1])*calcBSpline(x,knots,degree-1, offset+1);
    }
}

double calcBSpline(double x, const std::vector<double>& knots) {
    return calcBSpline(x, knots, knots.size()-2ul, 0);
}