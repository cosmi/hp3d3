//
//  IGANode.hpp
//  HP-spline
//
//  Created by Marcin Skotniczny on 24.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef IGANode_hpp
#define IGANode_hpp

#include "Node.h"
#include "CellId.h"
#include "Mesh.h"
#include "MapCollection.h"
#include "BSpline.hpp"
#include <vector>

template<int DEG, int DIMS>
class IGANode: public Node<DIMS> {
    
};


template<int DIMS>
class IGANode<2, DIMS> : public Node<DIMS>{
    using Element = Element<DIMS>;
    Element* anchor;
    CellId<DIMS> bounds;
    std::vector<double> knots[DIMS];
public:
    IGANode(Element* anchor, const Mesh<DIMS>& mesh):anchor(anchor), bounds(CellId<DIMS>::null()) {
        // This works only on quadlike, tau-rule meshes:
        auto neibors = mapping(anchor->getNeighbors(), [](Element* el){return el->getBounds();});
        bounds = neibors.begin() != neibors.end()?CellId<DIMS>::getBounds(neibors.begin(), neibors.end()): anchor->getBounds();
        bounds = CellId<DIMS>::getBounds(bounds, anchor->getBounds());
        auto supp = mesh.getAllElementsInBounds(bounds, anchor);
        
        Node<DIMS>::support.insert(supp.begin(), supp.end());
        
        
        FOR(i, DIMS) {
            knots[i].resize(4);
            knots[i][0] = bounds.getFrom()[i];
            knots[i][1] = anchor->getBounds().getFrom()[i];
            knots[i][2] = anchor->getBounds().getTo()[i];
            knots[i][3] = bounds.getTo()[i];
//            if(knots[i][0]==knots[i][1]) {
//                knots[i][0] -= knots[i][2] - knots[i][1];
//            }
//            if(knots[i][2]==knots[i][3]) {
//                knots[i][3] += knots[i][2] - knots[i][1];
//            }
        }
    }
    ~IGANode(){}
    CellId<DIMS> getBounds() const {
        return bounds;
    }
    const auto& getAnchorElement() const {
        return anchor;
    }
    CellId<DIMS> getAnchor() const {
        return anchor->getBounds();
    }
    
    double getValue(const Coordinate<DIMS>& x) const {
        double ret = 1;
        FOR(i, DIMS) {
            ret *= calcBSpline(x[i], knots[i]);
        }
        return ret;
    }
    double getKnot(int dim, int i) const {
        return knots[dim][i];
    }
};

template<int DIMS>
class IGANode<3, DIMS> : public Node<DIMS>{
    using Element = Element<DIMS>;
    CellId<DIMS> anchor;
    CellId<DIMS> bounds;
    
public:
    IGANode(const CellId<DIMS>& anchor, const Mesh<DIMS>& mesh):anchor(anchor), bounds(CellId<DIMS>::null()) {
        //TODO
        
        
    }
    ~IGANode(){}
    CellId<DIMS> getBounds() const {
        return bounds;
    }
    CellId<DIMS> getAnchor() const {
        return anchor;
    }
};



template <int DEG, int DIMS>
std::ostream& operator<<(std::ostream& os,
                         const IGANode<DEG,DIMS>& node) {
    
    
    os << "IGA[";
    os << node.getAnchor();
    FOR(i, DIMS) {
        os << ":";
        FOR(j, DEG+2) {
            os << node.getKnot(i,j) <<",";
        }
        
    }
    return os << ']';
}

#endif /* IGANode_hpp */
