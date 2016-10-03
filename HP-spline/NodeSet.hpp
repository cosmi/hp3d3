//
//  NodeSet.hpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 24.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef NodeSet_hpp
#define NodeSet_hpp

#include "Helpers.hpp"
#include "GeometryHelpers.hpp"
#include "Node.h"
#include "Mesh.h"
#include <unordered_map>
#include <vector>
#include "MapCollection.h"

template<int DIMS>
class NodeSet {
    using Node = Node<DIMS>;
    using Mesh = Mesh<DIMS>;
    std::unordered_map<Node*, double> nodes;
    const Mesh& mesh;
    NodeSet(const NodeSet& that) = delete;

public:
    NodeSet(const Mesh& mesh) : mesh(mesh){}
    
    ~NodeSet() {
        for(auto p: nodes) {
            delete p.first;
        }
    }
    
    void addNode(Node* node, double value = 0) {
        nodes[node] = 0;
    }
    template<class collection>
    void addNodes(const collection& coll) {
        for(Node* node: coll) {
            addNode(node);
        }
    }
    
    void setNodeValue(Node* node, double value) {
        auto tgt = nodes.find(node);
        assert(tgt != nodes.end());
        tgt->second = value;
    }
    
    double getValue(const Coordinate<DIMS>& x) {
        double v = 0;
        double div = 0;
        for(auto p: nodes) {
            double val = p.first->getValue(x);
            v += val*p.second;
            div += val;
        }
        return v/div;
    }
    
    std::vector<std::pair<Node*, double> >  getLinearDeps(const Coordinate<DIMS>& x) {
        std::vector<std::pair<Node*, double> > ret;
        
        for(auto p: nodes) {
            double v = p.first->getValue(x);
            if(!isZero(v)) {
                ret.push_back(std::make_pair(p.first, v));
            }
        }
        return ret;
    }
    auto getNodes() const {
        return mapping(nodes, [](auto p){return p.first;});
    }
    const Mesh& getMesh() const {
        return mesh;
    }
};



#endif /* NodeSet_hpp */
