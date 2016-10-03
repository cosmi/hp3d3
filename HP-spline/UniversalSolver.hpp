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
#include "SVGMatrixRenderer.hpp"

template<int DIMS>
struct Sample {
    Coordinate<DIMS> coords;
    double val;
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
    renderAndOpen(sm);
    sm.resetSetCount();
//    sm.print(std::cout, nullptr);
    clockTick("Elimiate start");
    sm.eliminate(nullptr);
    clockTick("Elimiate end");
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
    std::cout << "Touched:\t" << touch <<"/" << dims << " (" << touch/dims << ")" << std::endl;
}

template <class Function, int DIMS>
void solveWithMidpoints(NodeSet<DIMS>& nset, const Function& fun) {
//    const Mesh<DIMS>& mesh = nset.getMesh();
    
    
    std::vector<Sample<DIMS>> samples;
    
    for(auto node : nset.getNodes()) {
        auto midpoint = getMidPoint(node->getAnchor());
        
        samples.emplace_back(Sample<DIMS>{midpoint, fun(midpoint)});
    }
    solveWithSamples(nset, samples);
}



#endif /* UniversalSolver_hpp */
