//
//  meshBuilders.h
//  HP-spline
//
//  Created by Marcin on 29.04.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef meshBuilders_h
#define meshBuilders_h

#include <vector>
#include "defs.h"
#include "Mesh.h"
#include "CellId.h"

template<int DIMS>
void ensureNeighborDegree(Mesh<DIMS> &mesh, Element<DIMS> *el) {
    std::vector<Element<DIMS>*> neighbors(el->getNeighbors().begin(), el->getNeighbors().end());
std::cerr << "HERELY "<< el->getNeighbors().size() << std::endl;
    while(!neighbors.empty()) {
                                                                std::cerr << "HEREST"<<std::endl;
        auto nei = neighbors.back();
        neighbors.pop_back();
        auto overlap = nei->getBounds().getOverlap(el->getBounds());
        auto side = nei->getBounds().getAdjacentSide(el->getBounds());
        FOR(i, DIMS) {
            if(side.getLength(i) > 2 * overlap.getLength(i)) {
                auto ret = mesh.split(el, i);
                for (auto child : {ret.first, ret.second}) {
                    neighbors.push_back(child);
                }
                break;
            }
        }
    }
}

template<int DIMS>
void refineMeshUntilBounds(Mesh<DIMS>& mesh, CellId<DIMS> bounds) {
    while(true) {
        bool change = false;
        for(auto el : mesh.getQuadTree().findElementsOverlappingBounds(bounds)) {
            auto bds = el->getBounds();
            if(!bounds.covers(bds)) {

                FOR(i, DIMS) {
                    if(bds.getTo()[i] > bounds.getTo()[i] || bds.getFrom()[i] < bounds.getFrom()[i]) {
                                                    std::cerr << "HER"<<std::endl;
                        auto children = mesh.split(el, i);
                        for(auto ch : {children.first, children.second} ) {
                                                        std::cerr << "HERER"<<std::endl;
                            ensureNeighborDegree(mesh, ch);
                        }
                        break;
                    }
                }
                change = true;
                break;
            }
        }
        if(!change) break;
    }
}


template<int DIMS>
void refineMeshUntilBoundsByQuadDivisions(Mesh<DIMS>& mesh, CellId<DIMS> bounds) {
    while(true) {
        bool change = false;
        for(auto el : mesh.getQuadTree().findElementsOverlappingBounds(bounds)) {
            auto bds = el->getBounds();
            if(!bounds.covers(bds)) {
                
                mesh.splitInAllDims(el);
                change = true;
                break;
            }
        }
        if(!change) break;
    }
}


template<int DIMS>
void enforceTauRule(Mesh<DIMS>& mesh, CellId<DIMS> bounds) {
    while(true) {
        bool change = false;
        for(auto el : mesh.getQuadTree().findElementsOverlappingBounds(bounds)) {
            if(!bounds.covers(el->getBounds())) {
                mesh.splitInAllDims(el);
                change = true;
            }
        }
        if(!change) break;
    }
}




#endif /* meshBuilders_h */
