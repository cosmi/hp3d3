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
#include "SparseMatrix.hpp"
#include "UniversalSolver.hpp"

#include "drawings.hpp"
#include "OrderingGenerator.hpp"

//void testBSpline() {
//    std::vector<double> knots({0,0,2,4});
//    
//    for(double i = -1; i<5; i+=0.1) {
//        std::cout << i << '\t' << calcBSpline(i, knots) <<std::endl;
//    }
//}

int main(int argc, const char * argv[]) {
//    drawAllStuff();
//    testBSpline();
//    testCanvas();
    using namespace std;
//    SparseMatrix<> sm;
//    
//    sm.set(0,0,2);
//    sm.set(0,1,1);
//    sm.set(1,0,1);
//    sm.set(1,1,1.5);
//    sm.set(0,2,5);
//    sm.set(1,2,3.5);
//    sm.print(cout,0,1,0,2);
//    cout << "!!!" << endl;
//    sm.eliminate(2);
//    sm.print(cout,0,1,0,2);
//    cout << "!!!" << endl;
//    
//    return 0;
    
    selfTest();
//    return 0;
    
    const int DIMS = 2;
    const int SIZE = 16;
    Mesh<DIMS> m(CellId<DIMS>::cube(SIZE));
//    for(int i = 0; i< SIZE; i+=1) {
    int i = 0;
        for(int j = 0; j<SIZE; j+=1) {
            refineMeshUntilBoundsByQuadDivisions(m, CellId<DIMS>({i,j,0,0}, {i+1,j+1,1,1}));
        }
//    }
//    refineMeshUntilBoundsByQuadDivisions(m, CellId<DIMS>::cube(1));
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
//    IGANode<2,DIMS>* testnode;
    for(auto n1: nset.getNodes()) {
        
        IGANode<2,DIMS>* node = dynamic_cast<IGANode<2,DIMS>*>(n1);
        nset.setNodeValue(n1, 1);
//        testnode = node;
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
    auto ordering = weightedBisectionOrdering(nset);
//    renderAndOpen(m, nset);
    auto fun = [](const Coordinate<DIMS>&x) {return 1.;/*/(x[0]+x[1]+1);*/};
    
    solveWithMidpointsInOrder(nset, fun, ordering);
//    solveWithMidpoints(nset, fun);
    
    
//    for(double i = testnode->getKnot(0,0); i<= testnode->getKnot(0,3); i+=0.2) {
    for(double i = 0.0; i<= 16.01; i+=0.5) {
//        cout << i << ":\t";
//        for(double j = testnode->getKnot(1,0); j<= testnode->getKnot(1,3); j+=0.2) {
        for(double j = 0.0; j<= 4.01; j+=0.5) {
            Coordinate<DIMS> x{i,j};
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
