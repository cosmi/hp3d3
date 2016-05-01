//
//  Element.h
//  HP-spline
//
//  Created by Marcin on 27.04.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef Element_h
#define Element_h
#include <set>
#include <list>
#include "CellId.h"

template<int DIMS>
class Element {
    CellId<DIMS> bounds;
    std::unordered_set<Element*> neighbors;
public:
    Element(const CellId<DIMS>& bounds):bounds(bounds){}
    auto& getBounds() const {
        return bounds;
    }
    auto& getNeighbors() const {
        return neighbors;
    }
    
    // Will autodestruct
    std::pair<Element*, Element*> split(int dim) {
        auto p = bounds.split(dim);
        auto * a = new Element(p.first);
        auto * b = new Element(p.second);
        disconnect();
        for (auto el : neighbors) {
            a->connectIfNeighboring(*el);
            b->connectIfNeighboring(*el);
        }
        delete this;
        return std::make_pair(a,b);
    }

    
    
    void disconnect() {
        for(auto el : neighbors) {
            el->neighbors.erase(this);
        }
    }
    
    void connectIfNeighboring(Element& neighbor) {
        if(getBounds().isNeighboring(neighbor.getBounds())) {
            neighbors.insert(&neighbor);
            neighbor.neighbors.insert(this);
        }
    }
    
    auto findCornerNeighbors() const {
        std::unordered_set<Element*> corners;
        for(auto el: neighbors) {
            for(auto el2 : el->getNeighbors()) {
                auto& bds = el2->getBounds();
                if(bounds.touches(bds) && neighbors.count(el2) == 0) {
                    corners.insert(el2);
                }
            }
        }
        return corners;
    }
};

#endif /* Element_h */
