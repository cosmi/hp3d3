//
//  Header.h
//  HP-spline
//
//  Created by Marcin on 10.05.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef HPNodes_h
#define HPNodes_h
#include "Node.h"

namespace HP {
    class InternalNode : public Node {
    public:
        InternalNode(Mesh& m, Element* el) {
            support.push_back(el);
        }
        InternalNode(Mesh& m, const CellId & cid) {
            auto ret = mesh.getQuadTree().findElementsOverlappingBounds(cid);
            assert(ret.size() == 1);
            support.push_back(ret.front());
        }
    };

    class EdgeNode : public Node {
    public:
        
        InternalNode(Mesh& m, const CellId & cid) {
            support.push_back(el);
        }
        

    };
}


#endif /* HPNodes_h */
