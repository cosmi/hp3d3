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


int main(int argc, const char * argv[]) {
//    testCanvas();
    selfTest();
    using namespace std;
    const int DIMS = 2;
    Mesh<DIMS> m(CellId<DIMS>({0,0,0},{4,4,4}));
    refineMeshUntilBounds(m, CellId<DIMS>({0,0,0}, {1,1,1}));
    renderAndOpen(m);
    cout << m.getQuadTree().findElementsOverlappingBounds(CellId<DIMS>({1,1,1},{3,3,3})).size() << endl;
    return 0;
}
