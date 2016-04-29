//
//  QuadTree.h
//  HP-spline
//
//  Created by Marcin on 27.04.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef QuadTree_h
#define QuadTree_h
#include <vector>

#include <unordered_set>
#include "CellId.h"

template<int DIMS, class Element>
class QuadTree {
    CellId<DIMS> bounds;
    std::unordered_set<Element*> elements;
    
    QuadTree* children[1<<DIMS];
public:
    bool isLeaf() {
        return !children[0];
    }
    void addElement(Element* el) {
        auto eb = el->getBounds();
        if(eb.overlaps(bounds)) {
            elements.insert(el);
        }
    }
    void removeElement(Element* el) {
        elements.erase(el);
    }
    auto findElementsOverlappingBounds(const CellId<DIMS>& bds) const {
        std::unordered_set<Element*> ret;
        for( auto el : elements) {
            if(el->getBounds().overlaps(bds)) {
                ret.insert(el);
            }
        }
        return ret;
    }
    auto findElementsCoveringBounds(const CellId<DIMS>& bds) const {
        std::unordered_set<Element*> ret;
        for( auto el : elements) {
            if(el->getBounds().covers(bds)) {
                ret.insert(el);
            }
        }
        return ret;
    }
    
    auto getBounds() const {
        return bounds;
    }
    QuadTree(CellId<DIMS> bounds): bounds(bounds){};
};

#endif /* QuadTree_h */
