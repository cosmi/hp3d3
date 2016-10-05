//
//  UniversalSolver.hpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 24.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef UniversalSolver_hpp
#define UniversalSolver_hpp

#include <iterator>
#include "NodeSet.hpp"
#include "SparseMatrix.hpp"
#include "GeometryHelpers.hpp"
#include "nodeBuilders.h"
#include "IGANode.hpp"
#include "SVGMatrixRenderer.hpp"
#include "OrderingGenerator.hpp"

template<int DIMS>
struct Sample {
    Coordinate<DIMS> coords;
    double val;
    Node<DIMS>* idx;
//    template<class CollType>
//    Sample(const CollType& dims, double val):val(val) {
//        int i = 0;
//        for(auto it = std::begin(dims); i<DIMS; ++it, ++i) {
//            coords[i] = *it;
//        }
//    }
};

template<class SampleColl, int DIMS>
void solveWithSamples(NodeSet<DIMS>& nset, const SampleColl& samples) {
    
    SparseMatrix<Node<DIMS>*, Node<DIMS>*, double, less_by_ptr<Node<DIMS>*>, less_by_ptr<Node<DIMS>*>> sm;
    
    
    for(const Sample<DIMS>& s: samples) {
        auto deps = nset.getLinearDeps(s.coords);
        
        for(auto dep: deps) {
            sm.set(s.idx, dep.first, dep.second);
        }
        sm.set(s.idx, nullptr, s.val);
    }
    renderAndOpen(sm);
    sm.resetSetCount();
//    sm.print(std::cout, nullptr);
    clockTick("Eliminate start");
    sm.eliminate(nullptr);
    clockTick("Eliminate end");
//    std::cout << "#\n";
//    sm.print(std::cout, nullptr);
//    std::cout << "#\n\n";
    renderAndOpen(sm);
    auto results = sm.getResults(nullptr);
    for(auto it: results) {
        nset.setNodeValue(it.first, it.second);
    }
    std::cout << "OPs:  \t" << sm.getSetCount() << std::endl;
    std::cout << "Rows: \t " << sm.getRowsCount() << std::endl;
    std::cout << "Cols: \t" << sm.getColsCount() << std::endl;
    auto touch = sm.getSetFieldsCount();
    auto dims = sm.getRowsCount()*sm.getColsCount();
    std::cout << "Touched:\t" << touch <<"/" << dims << " (" << touch/(double)dims << ")" << std::endl;
}

template <class Function, int DIMS>
void solveWithMidpoints(NodeSet<DIMS>& nset, const Function& fun) {
//    const Mesh<DIMS>& mesh = nset.getMesh();
    
    
    std::vector<Sample<DIMS>> samples;
    
    for(auto node : nset.getNodes()) {
        auto midpoint = getMidPoint(node->getAnchor());
        
        samples.emplace_back(Sample<DIMS>{midpoint, fun(midpoint), node});
    }
    solveWithSamples(nset, samples);
}



template<class SampleColl, int DIMS>
void solveWithSamplesInOrder(NodeSet<DIMS>& nset, const SampleColl& samples, const std::vector<Node<DIMS>*>& order) {
    
    SparseMatrix<Node<DIMS>*, Node<DIMS>*, double, less_by_ptr<Node<DIMS>*>, less_by_ptr<Node<DIMS>*>> sm;
    
    
    for(const Sample<DIMS>& s: samples) {
        auto deps = nset.getLinearDeps(s.coords);
        
        for(auto dep: deps) {
            sm.set(s.idx, dep.first, dep.second);
        }
        sm.set(s.idx, nullptr, s.val);
    }
    renderAndOpen(sm);
    sm.resetSetCount();
    //    sm.print(std::cout, nullptr);
    clockTick("Eliminate start");
    sm.eliminateInOrder(nullptr, order);
    clockTick("Eliminate end");
    //    std::cout << "#\n";
    //    sm.print(std::cout, nullptr);
    //    std::cout << "#\n\n";
    renderAndOpen(sm);
    auto results = sm.getResults(nullptr);
    for(auto it: results) {
        nset.setNodeValue(it.first, it.second);
    }
    std::cout << "OPs:  \t" << sm.getSetCount() << std::endl;
    std::cout << "Rows: \t " << sm.getRowsCount() << std::endl;
    std::cout << "Cols: \t" << sm.getColsCount() << std::endl;
    auto touch = sm.getSetFieldsCount();
    auto dims = sm.getRowsCount()*sm.getColsCount();
    std::cout << "Touched:\t" << touch <<"/" << dims << " (" << touch/(double)dims << ")" << std::endl;
}

template <class Function, int DIMS>
void solveWithMidpointsInOrder(NodeSet<DIMS>& nset, const Function& fun, const std::vector<Node<DIMS>*>& order) {
    //    const Mesh<DIMS>& mesh = nset.getMesh();
    
    
    std::vector<Sample<DIMS>> samples;
    
    for(auto node : nset.getNodes()) {
        auto midpoint = getMidPoint(node->getAnchor());
        
        samples.emplace_back(Sample<DIMS>{midpoint, fun(midpoint), node});
    }
    solveWithSamplesInOrder(nset, samples, order);
}




template <int DIMS>
void constructAndRenderMatrixWithMidpoints(Mesh<DIMS>& mesh, const char* filename = nullptr) {
    //    const Mesh<DIMS>& mesh = nset.getMesh();
    
    NodeSet<DIMS> nset(mesh);
    nset.addNodes(generateElementBasedNodes<DIMS, IGANode<2, DIMS>>(mesh));
    
    auto fun = [](const Coordinate<DIMS>&x) {return 1.;/*/(x[0]+x[1]+1);*/};
    
    std::vector<Sample<DIMS>> samples;
    
    for(auto node : nset.getNodes()) {
        auto midpoint = getMidPoint(node->getAnchor());
        
        samples.emplace_back(Sample<DIMS>{midpoint, fun(midpoint)});
    }
    
    SparseMatrix<int, Node<DIMS>*, double, std::less<int>, less_by_ptr<Node<DIMS>*>> sm;
    
    int sampleIdx = 0;
    for(const Sample<DIMS>& s: samples) {
        auto deps = nset.getLinearDeps(s.coords);
        
        for(auto dep: deps) {
            sm.set(sampleIdx, dep.first, dep.second);
        }
        sm.set(sampleIdx, nullptr, s.val);
        sampleIdx++;
    }
    renderAndOpen(sm, filename);
}

//
//
//template <int DIMS>
//void constructAndRenderMatrixWithMidpointsInBWOrder(Mesh<DIMS>& mesh, const char* filename = nullptr) {
//    //    const Mesh<DIMS>& mesh = nset.getMesh();
//    
//    NodeSet<DIMS> nset(mesh);
//    nset.addNodes(generateElementBasedNodes<DIMS, IGANode<2, DIMS>>(mesh));
//    auto ordering = weightedBisectionOrdering(nset);
//    
//    auto fun = [](const Coordinate<DIMS>&x) {return 1.;/*/(x[0]+x[1]+1);*/};
//    
//    std::vector<Sample<DIMS>> samples;
//    
//    for(auto node : nset.getNodes()) {
//        auto midpoint = getMidPoint(node->getAnchor());
//        
//        samples.emplace_back(Sample<DIMS>{midpoint, fun(midpoint)});
//    }
//    
//    SparseMatrix<Node<DIMS>*, Node<DIMS>*, double, std::less<int>, less_by_ptr<Node<DIMS>*>> sm;
//    
//    for(const Sample<DIMS>& s: samples) {
//        auto deps = nset.getLinearDeps(s.coords);
//        
//        for(auto dep: deps) {
//            sm.set(s.idx, dep.first, dep.second);
//        }
//        sm.set(s.idx, nullptr, s.val);
//    }
//    renderAndOpen(sm, filename);
//}




#endif /* UniversalSolver_hpp */
