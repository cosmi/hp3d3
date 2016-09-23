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
#include "FilterCollection.h"



int main(int argc, const char * argv[]) {
//    testCanvas();
//    std::vector<int> V({1,2,3});
//    auto f = filter<int>(V, [](int x){return x%2 == 0;});
//    for(auto i : f) {
//        std::cout << i << std::endl;
//    }
    
    selfTest();
    using namespace std;
    const int DIMS = 2;
    Mesh<DIMS> m(CellId<DIMS>({0,0,0},{16,16,16}));
    refineMeshUntilBoundsByQuadDivisions(m, CellId<DIMS>({4,4,4}, {8,5,8}));
    assert(isQuadLikeMesh(m));
    renderAndOpen(m);
    ensureTauRuleForQuadLikeMesh(m);
    
    
    // does not work here –> probably some neighbors links are being lost
    // TODO: write neighbor graph integrity checker
    assert(verifyNeighborGraphIntegrity(m));
    renderAndOpen(m);
    assert(isQuadLikeTauRuleMesh(m));
    
    cout << m.getQuadTree().findElementsOverlappingBounds(CellId<DIMS>({1,1,1},{3,3,3})).size() << endl;
    return 0;
}
