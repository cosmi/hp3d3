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
#include "MapCollection.h"
#include "IGANode.hpp"
#include "nodeBuilders.h"



int main(int argc, const char * argv[]) {
//    testCanvas();
    
    
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
    using Node = IGANode<2, DIMS> ;
    auto nodes = generateElementBasedNodes<DIMS, Node >(m);
    
    for(auto node: nodes) {
        for(auto el: node.getSupport()) {
            el->label = el->label+"*";
        }
    }
    renderAndOpen(m, nodes);
    
    cout << m.getQuadTree().findElementsOverlappingBounds(CellId<DIMS>({1,1,1},{3,3,3})).size() << endl;
    return 0;
}
