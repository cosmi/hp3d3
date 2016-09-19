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
    const int DIMS = 3;
    Mesh<DIMS> m(CellId<DIMS>({0,0,0},{16,16,16}));
    refineMeshUntilBoundsByQuadDivisions(m, CellId<DIMS>({4,4,4}, {8,5,8}));
    assert(isQuadLikeMesh(m));
    ensureTauRuleForQuadLikeMesh(m);
    
    // does not work here –> probably some neighbors links are being lost
    // TODO: write neighbor graph integrity checker
    assert(isQuadLikeTauRuleMesh(m));
    renderAndOpen(m);
    cout << m.getQuadTree().findElementsOverlappingBounds(CellId<DIMS>({1,1,1},{3,3,3})).size() << endl;
    return 0;
}
