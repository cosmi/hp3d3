//
//  OrderingGenerator.hpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 05.10.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef OrderingGenerator_hpp
#define OrderingGenerator_hpp

#include "Node.h"
#include "NodeSet.hpp"
#include <vector>
#include <unordered_set>
#include "FilterCollection.h"
#include <array>


template<int DIMS>
std::array<std::vector<Node<DIMS>*>, 2> splitAgainst(const std::vector<Node<DIMS>*>& nodes, CellId<DIMS> bounds) {
    auto nodes1 = filter(nodes, [bounds](Node<DIMS>* node) {
        return bounds.covers(node->getAnchor());
    });
    auto nodes2 = filter(nodes, [bounds](Node<DIMS>* node) {
        return !bounds.stronglyOverlaps(node->getAnchor());
    });
    if(nodes1.size() + nodes2.size() == nodes.size()) {
        return std::array<std::vector<Node<DIMS>*>, 2>{nodes1, nodes2};
    } else {
        std::cout << "Nada\n";
        return std::array<std::vector<Node<DIMS>*>, 2>();
        
    }
}

template<int DIMS>
size_t calculateBisectionCost(const std::vector<Node<DIMS>*>& nodes1, const std::vector<Node<DIMS>*>& nodes2) {
    size_t result = 0;
    for(auto n1: nodes1) {
        for(auto n2: nodes2) {
            if(n1->getAnchor().getNeighborhoodLevel(n2->getAnchor()) >= 0) {
                result++;
            }
        }
    }
    return result;
}


template<int DIMS>
void weightedBisectionOrderingHelper(const std::vector<Node<DIMS>*>& nodes, CellId<DIMS> bounds, std::vector<Node<DIMS>*>& result) {
    std::cout << nodes.size() << ' ' << bounds << ' ' << (*nodes.begin())->getAnchor() << std::endl;
    assert(nodes.size() > 1 || bounds == (*nodes.begin())->getAnchor());
    
    if(nodes.size() > 1) {
        int bestDim = -1;
        size_t bestCost = 0;
        FOR(i, DIMS) if(bounds.getSize()[i] > 1) {
            auto s = splitAgainst(nodes, bounds.getHalf(i));
            if(s[0].size() == 0) continue;
            
            auto cost = calculateBisectionCost(s[0], s[1]);
            if(bestDim == -1 || bestCost > cost) {
                bestDim = i;
                bestCost = cost;
            }
            
        }
        assert(bestDim >= 0);
        
        
        auto half = bounds.getHalf(bestDim);
        auto half2 = bounds.getSecondHalf(bestDim);
        auto s = splitAgainst(nodes, half);
        
        
        weightedBisectionOrderingHelper(s[0], half, result);
        weightedBisectionOrderingHelper(s[1], half2, result);
        
        
        for(auto node: nodes) {
            if(bounds.covers(node->getBounds()) && !half.covers(node->getBounds()) && half2.covers(node->getBounds())) {
                result.push_back(node);
            }
        }
        
    } else {
        if(bounds.covers((*nodes.begin())->getBounds())) {
            result.push_back(*nodes.begin());
        }
    }
    
    
}

template<int DIMS>
std::vector<Node<DIMS>*> weightedBisectionOrdering(const NodeSet<DIMS>& nset) {
    const Mesh<DIMS>& mesh = nset.getMesh();
    using Set = std::unordered_set<Node<DIMS>*>;
    
    auto nodes1 = nset.getNodes();
    std::vector<Node<DIMS>*> nodes;
    for(auto node: nodes1) {
        nodes.push_back(node);
    }
    
    CellId<DIMS> bounds = mesh.getBounds();
    std::vector<Node<DIMS>*> result;
    weightedBisectionOrderingHelper(nodes, bounds, result);
    
    return result;
}

#endif /* OrderingGenerator_hpp */
