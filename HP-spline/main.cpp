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

#include "BSpline.hpp"
#include "NodeSet.hpp"

//void testBSpline() {
//    std::vector<double> knots({1,2,5});
//    
//    for(double i = 0; i<7; i+=0.1) {
//        std::cout << i << '\t' << calcBSpline(i, knots) <<std::endl;
//    }
//}

int main(int argc, const char * argv[]) {
//    testBSpline();
//    testCanvas();
    
    
    selfTest();
    using namespace std;
    const int DIMS = 2;
    Mesh<DIMS> m(CellId<DIMS>({0,0,0},{16,16,16}));
//    for(int i = 0; i< 16; i++) {
//        for(int j = 0; j<16; j++) {
//            refineMeshUntilBoundsByQuadDivisions(m, CellId<DIMS>({i,j,4}, {i+1,j+1,8}));
//        }
//    }
    refineMeshUntilBoundsByQuadDivisions(m, CellId<DIMS>({0,0}, {1,1}));
//    refineMeshUntilBoundsByQuadDivisions(m, CellId<DIMS>({4,4,4}, {8,8,8}));
//    refineMeshUntilBoundsByQuadDivisions(m, CellId<DIMS>({8,8,8}, {12,12,12}));
    assert(isQuadLikeMesh(m));
//    renderAndOpen(m);
    ensureTauRuleForQuadLikeMesh(m);
    
    // does not work here –> probably some neighbors links are being lost
    // TODO: write neighbor graph integrity checker
    assert(verifyNeighborGraphIntegrity(m));
//    renderAndOpen(m);
    assert(isQuadLikeTauRuleMesh(m));
    using Node = IGANode<2, DIMS> ;
    NodeSet<DIMS> nset(m);
    nset.addNodes(generateElementBasedNodes<DIMS, Node >(m));
    IGANode<2,DIMS>* testnode;
    for(auto n1: nset.getNodes()) {
        
        IGANode<2,DIMS>* node = dynamic_cast<IGANode<2,DIMS>*>(n1);
        if(node->getAnchor() ==CellId<DIMS>({1,1},{2,2})) {
            nset.setNodeValue(n1, 1);
        }
        testnode = node;
//        FOR(i, int(DIMS)) {
//            FOR(j, 4) {
//                cout << node->getKnot(i,j) << endl;
//            }
//        }
//        cout << n1->getValue({15.,14.}) << " " << n1->getValue({15.,14.5}) << " " << n1->getValue({15.,15.}) << endl;
//        cout << n1->getValue({15.5,14.}) << " " << n1->getValue({15.5,14.5}) << " " << n1->getValue({15.4,15.}) << endl;
//        cout << n1->getValue({16.,14.}) << " " << n1->getValue({16.,14.5}) << " " << n1->getValue({16.,15.}) << endl;
//        break;
    }
    
    renderAndOpen(m, nset);
    
    
    
//    for(double i = testnode->getKnot(0,0); i<= testnode->getKnot(0,3); i+=0.2) {
    for(double i = 0; i<= 16.01; i+=0.2) {
//        cout << i << ":\t";
//        for(double j = testnode->getKnot(1,0); j<= testnode->getKnot(1,3); j+=0.2) {
        for(double j = 0; j<= 4.01; j+=0.2) {
            double x[]={i,j};
//            auto&& deps = nset.getLinearDeps(x);
//            double dep = 0;
//            for(auto& p: deps) {
//                dep+=p.second;
//            }
//            if(nset.getValue(x) >0) {
//                cout << "{"<<i <<','<<j <<"}";
//                for(auto& p: deps) {
//                    cout << *dynamic_cast<IGANode<2,DIMS>*>(p.first) <<"#" << p.second;
//                }
//                cout << endl;
//            }
            
            cout << nset.getValue(x) << "\t";
        }
        cout << endl;
    }
    
//    cout << m.getQuadTree().findElementsOverlappingBounds(CellId<DIMS>({1,1,1},{3,3,3})).size() << endl;
    return 0;
}
