//
//  main.cpp
//  HP-spline
//
//  Created by Marcin on 27.04.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#include <iostream>
#include "Mesh.h"
#include "meshBuilders.h"

#include "test.h"
#include "SVGCanvas.h"
#include "SVGRenderer.h"
#include "projections.hpp"

//void testCanvas() {
//    SVGRenderer<3, PerspectiveProjection> r("/tmp/canvas.svg", CellId<3>({0,0,0},{4,4,4}));
//    r.drawBounds(CellId<3>({0,0,0},{4,4,4}));
//    r.close();
//    r.openImage();
//}

int main(int argc, const char * argv[]) {
//    testCanvas();
    selfTest();
    using namespace std;
    const int DIMS = 2;
    Mesh<DIMS> m(CellId<DIMS>({0,0,0},{4,4,4}));
    refineMeshUntilBounds(m, CellId<DIMS>({0,0,0}, {1,1,1}));
    renderAndOpen(m);
    std::cout << m.getElements().size() << "Hello, World!\n";
    for (auto el : m.getElements()) {
        std:: cout << el->getBounds() << std::endl;
    }
    cout << m.getQuadTree().findElementsOverlappingBounds(CellId<DIMS>({1,1,1},{3,3,3})).size() << endl;
    return 0;
}
