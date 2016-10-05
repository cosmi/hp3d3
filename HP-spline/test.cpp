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
#include "FilterCollection.h"
#include "MapCollection.h"

//
//void testMeshBuilders() {
//    Mesh<3> m(CellId<3>({0,0,0},{4,4,4}));
//    refineMeshUntilBounds(m, CellId<3>({0,0,0}, {1,1,1}));
//    assert(m.getElements().size() == 15);
//}

void testFilterAndMap() {
    std::vector<int> V({1,2,3});
    auto f = mapping(filter(V, [](int x){return x%2 == 0;}), [](int x){return 2*x;});
//    for(auto i : f) {
//        std::cout << i << std::endl;
//    }
    assert(*(f.begin()) == 4);
}

void testMeshBuilders2() {
    {
        Mesh<3> m(CellId<3>({0,0,0},{4,4,4}));
        CellId<3> target({0,0,0}, {1,1,1});
        refineMeshUntilBoundsByQuadDivisions(m, target);
        Element<3>* el = nullptr;
        for(auto el1: m.getElements()) {
            if(el1->getBounds() == target) el = el1;
        }
        assert(el != nullptr);
        
        assert(m.getAllElementsInBounds(CellId<3>({0,0,0}, {2,2,2}), el).size() == 8);
        assert(m.getAllElementsInBounds(CellId<3>({0,0,0}, {3,3,3}), el).size() == 15);
        
        assert(m.getElements().size() == 15);
//        renderAndOpen(m);
    }
    {
        const int DIMS = 2;
        Mesh<DIMS> m(CellId<DIMS>({0,0},{16,16}));
        refineMeshUntilBoundsByQuadDivisions(m, CellId<DIMS>({4,5,6}, {5,6,7}));
        assert(isQuadLikeMesh(m));
        ensureTauRuleForQuadLikeMesh(m);
        assert(isQuadLikeTauRuleMesh(m));
        assert(m.getElements().size()==25);
    }
    {
        const int DIMS = 3;
        Mesh<DIMS> m(CellId<DIMS>({0,0,0},{16,16,16}));
        refineMeshUntilBoundsByQuadDivisions(m, CellId<DIMS>({4,5,6}, {5,6,7}));
        assert(isQuadLikeMesh(m));
        ensureTauRuleForQuadLikeMesh(m);
        assert(isQuadLikeTauRuleMesh(m));
        assert(m.getElements().size()==85);
    }
    {
        const int DIMS = 4;
        Mesh<DIMS> m(CellId<DIMS>({0,0,0,0},{16,16,16,16}));
        refineMeshUntilBoundsByQuadDivisions(m, CellId<DIMS>({4,5,6,7}, {5,6,7,8}));
        assert(isQuadLikeMesh(m));
        ensureTauRuleForQuadLikeMesh(m);
        assert(isQuadLikeTauRuleMesh(m));
        assert(m.getElements().size()==256);
    }
}

void selfTest() {
    testFilterAndMap();
    testMeshBuilders2();
}

