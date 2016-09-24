//
//  nodeBuilders.h
//  HP-spline
//
//  Created by Marcin Skotniczny on 24.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef nodeBuilders_h
#define nodeBuilders_h

#include "Mesh.h"
#include <list>

template<int DIMS, class Node>
std::list<Node> generateElementBasedNodes(const Mesh<DIMS>& mesh) {
    std::list<Node> ret;
    for(auto el: mesh.getElements()) {
        ret.emplace_front(Node(el, mesh));
    }
    return ret;
}




#endif /* nodeBuilders_h */
