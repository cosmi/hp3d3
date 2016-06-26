//
//  test.cpp
//  HP-spline
//
//  Created by Marcin on 29.04.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#include "test.h"
#include "Mesh.h"
#include "meshBuilders.h"

//
//void testMeshBuilders() {
//    Mesh<3> m(CellId<3>({0,0,0},{4,4,4}));
//    refineMeshUntilBounds(m, CellId<3>({0,0,0}, {1,1,1}));
//    assert(m.getElements().size() == 15);
//}

void testMeshBuilders2() {
    Mesh<3> m(CellId<3>({0,0,0},{4,4,4}));
    refineMeshUntilBoundsByQuadDivisions(m, CellId<3>({0,0,0}, {1,1,1}));
    assert(m.getElements().size() == 15);
}

void selfTest() {
    testMeshBuilders2();
}

