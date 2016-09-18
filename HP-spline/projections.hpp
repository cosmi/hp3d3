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
#include <cmath>

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

struct PerspectiveProjection {
    struct Coords {
        double x, y, z;
        Coords(double x, double y, double z):x(x),y(y), z(z){}
        Coords() = delete;
    } camera, angle;
    PerspectiveProjection():camera(-15,-20, -40), angle(0,0,0) {}
    PerspectiveProjection(double cx, double cy, double cz, double ax=0, double ay=0, double az=0): camera(cx, cy, cz), angle(ax, ay, az){}
    
    
    SVGPoint operator()(const Point<3>& pt) const {
        double ptx = pt[0], pty=pt[1], ptz=pt[2];
        double sx = sin(angle.x), sy = sin(angle.y), sz = sin(angle.z);
        double cx = cos(angle.x), cy = cos(angle.x), cz = cos(angle.z);
        double x = ptx - camera.x, y = pty - camera.y, z = ptz-camera.z;
        double ex = -camera.x, ey = -camera.y, ez = -camera.z;
        double dt = (cy*z + sy*(sz*y + cz*x));
        
        
        double dx = cy*(sz*y + cz*x) - sy*z;
        double dy = sx*dt + cx*(cz*y - sz*x);
        double dz = cx*dt - sx*(cz*y - sz*x);
        double bx = ez/dz*dx - ex;
        double by = ez/dz*dy - ey;
        return SVGPoint(bx, by);
    }
};

#endif /* projections_hpp */
