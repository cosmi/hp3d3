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
#include "CellId.h"
#include <cmath>

struct ObliqueProjection {
    double zx, zy;
    ObliqueProjection(double zx=0.35, double zy=0.25): zx(zx), zy(zy) {}
    
    SVGPoint operator()(const Point<3>& p) const {
        return SVGPoint(p[0] + p[2]*zx, p[1]+p[2]*zy);
    }
    SVGPoint operator()(const CellId<3>& cid) const {
        double x = (cid.getFrom()[0]+cid.getTo()[0])/2.;
        double y = (cid.getFrom()[1]+cid.getTo()[1])/2.;
        double z = (cid.getFrom()[2]+cid.getTo()[2])/2.;
        return SVGPoint(x+z*zx, y+z*zy);
    }
};


struct IdentityProjection {
    SVGPoint operator()(const Point<2>& p) const {
        return SVGPoint(p[0], p[1]);
    }
    SVGPoint operator()(const CellId<2>& cid) const {
        double x = (cid.getFrom()[0]+cid.getTo()[0])/2.;
        double y = (cid.getFrom()[1]+cid.getTo()[1])/2. ;
        return SVGPoint(x, y);
    }
};

struct PerspectiveProjection {
    struct Coords {
        double x, y, z;
        Coords(double x, double y, double z):x(x),y(y), z(z){}
        Coords(const Point<3>&pt):x(pt[0]), y(pt[1]), z(pt[2]){};
        Coords() = delete;
    } camera, angle;
    PerspectiveProjection():camera(-15,-20, -40), angle(0,0,0) {}
    PerspectiveProjection(double cx, double cy, double cz, double ax=0, double ay=0, double az=0): camera(cx, cy, cz), angle(ax, ay, az){}
    
    
    SVGPoint operator()(const Coords& pt) const {
        
        double sx = sin(angle.x), sy = sin(angle.y), sz = sin(angle.z);
        double cx = cos(angle.x), cy = cos(angle.x), cz = cos(angle.z);
        double x = pt.x - camera.x, y = pt.y - camera.y, z = pt.z-camera.z;
        double ex = -camera.x, ey = -camera.y, ez = -camera.z;
        double dt = (cy*z + sy*(sz*y + cz*x));
        
        
        double dx = cy*(sz*y + cz*x) - sy*z;
        double dy = sx*dt + cx*(cz*y - sz*x);
        double dz = cx*dt - sx*(cz*y - sz*x);
        double bx = ez/dz*dx - ex;
        double by = ez/dz*dy - ey;
        return SVGPoint(bx, by);
    }
    SVGPoint operator()(const CellId<3>& cellId) {
        return operator()(Coords(
                                 (cellId.getTo()[0]+cellId.getFrom()[0])/2.,
                                 (cellId.getTo()[1]+cellId.getFrom()[1])/2.,
                                 (cellId.getTo()[2]+cellId.getFrom()[2])/2.));
    }
};

#endif /* projections_hpp */
