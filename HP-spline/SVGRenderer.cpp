//
//  SVGRenderer.cpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 18.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#include "SVGRenderer.h"

#include <string>

static char openCounter = 1;

std::string getTmpCanvasFilename() {
    return "/tmp/canvas" + toString((int)(openCounter++)) + ".svg";
}

void renderAndOpen(const Mesh<2>& mesh, const char* filename) {
    std::string s = filename?filename:getTmpCanvasFilename();
    SVGRenderer<2> r(s.c_str(), mesh.getBounds());
    r.drawMesh(mesh);
    r.close();
    r.openImage();
}



void renderAndOpen(const Mesh<3>& mesh, const char* filename, bool onlyFasade) {
    std::string s = filename?filename:getTmpCanvasFilename();
    SVGRenderer<3, PerspectiveProjection> r(s.c_str(), mesh.getBounds());
    r.drawMesh(mesh, onlyFasade);
    r.close();
    r.openImage();
}

void renderAndOpen(const Mesh<4>& mesh, const char* filename) {
    std::string s = filename?filename:getTmpCanvasFilename();
    SVGRenderer<4, PerspectiveProjection> r(s.c_str(), mesh.getBounds());
    r.drawMesh4D(mesh);
    r.close();
    r.openImage();
}


template<> std::pair<SVGPoint, SVGPoint> calculateBounds<4, PerspectiveProjection> (const CellId<4>& cid, const PerspectiveProjection& p) {
    auto points = cid.getLowerDimensionalityBounds(0);
    bool first = true;
    SVGPoint ptMin(0,0), ptMax(0,0);
    for(auto & point: points) {
        if(first) {
            ptMin = ptMax = p(point.getFrom());
            first = false;
        } else {
            auto pt2 = p(point.getFrom());
            if(pt2.x < ptMin.x) ptMin.x = pt2.x;
            if(pt2.y < ptMin.y) ptMin.y = pt2.y;
            if(pt2.x > ptMax.x) ptMax.x = pt2.x;
            if(pt2.y > ptMax.y) ptMax.y = pt2.y;
        }
    }
    
    double xdiff = ptMax.x - ptMin.x;
    double ydiff = ptMax.y - ptMin.y;
//    ptMin.x-=xdiff;
    ptMax.x+=xdiff*1.1;
//    ptMin.y-=ydiff;
    ptMax.y+=ydiff*1.1;
    
    return std::make_pair(ptMin, ptMax);
}
