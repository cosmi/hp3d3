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

template<int DEG, int DIMS>
class IGANode: public Node<DIMS> {
    
};


template<int DIMS>
class IGANode<2, DIMS> : public Node<DIMS>{
    using Element = Element<DIMS>;
    Element* anchor;
    CellId<DIMS> bounds;
    
public:
    IGANode(Element* anchor, const Mesh<DIMS>& mesh):anchor(anchor), bounds(CellId<DIMS>::null()) {
        // This works only on quadlike, tau-rule meshes:
        auto neibors = mapping(anchor->getNeighbors(), [](Element* el){return el->getBounds();});
        bounds = CellId<DIMS>::getBounds(neibors.begin(), neibors.end());
        auto supp = mesh.getAllElementsInBounds(bounds, anchor);
        
        Node<DIMS>::support.insert(supp.begin(), supp.end());
//        std::cout << "+" << anchor->getBounds() << " has " << Node<DIMS>::support.size() << std::endl;
//        for(auto s : supp) {
//            std::cout << s << ":" << s->getBounds() << "   ";
//        }
//        std::cout <<std::endl;
    }
    ~IGANode(){}
    const auto& getBounds() const {
        return bounds;
    }
    const auto& getAnchor() const {
        return anchor;
    }
};

#endif /* IGANode_hpp */
