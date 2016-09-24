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



void renderAndOpen(const Mesh<3>& mesh, const char* filename) {
    std::string s = filename?filename:getTmpCanvasFilename();
    SVGRenderer<3, PerspectiveProjection> r(s.c_str(), mesh.getBounds());
    r.drawMesh(mesh);
    r.close();
    r.openImage();
}
