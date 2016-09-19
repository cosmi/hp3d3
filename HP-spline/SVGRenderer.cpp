//
//  SVGRenderer.cpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 18.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#include "SVGRenderer.h"
void renderAndOpen(const Mesh<2>& mesh, const char* filename) {
    SVGRenderer<2> r(filename, mesh.getBounds());
    r.drawMesh(mesh);
    r.close();
    r.openImage();
}

void renderAndOpen(const Mesh<3>& mesh, const char* filename) {
    SVGRenderer<3, PerspectiveProjection> r(filename, mesh.getBounds());
    r.drawMesh(mesh);
    r.close();
    r.openImage();
}
