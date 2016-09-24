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
#include "FilterCollection.h"

template<int DIMS>
class Element {
    CellId<DIMS> bounds;
    std::unordered_set<Element*> neighbors;
    
public:
    std::string label;
    Element(const CellId<DIMS>& bounds):bounds(bounds){}
    const CellId<DIMS>& getBounds() const {
        return bounds;
    }
    auto& getCornerNeighbors() const {
        return neighbors;
    }
    auto getNeighbors() const {
        return filter<Element*>(neighbors, [this](Element*el) {
            return el->getBounds().isNeighboring(this->getBounds());
        });
    }
    
    // Will autodestruct
    std::pair<Element*, Element*> split(int dim) {
        
        auto p = bounds.split(dim);
        assert(p.first.isFullyDimensional());
        assert(p.second.isFullyDimensional());
        auto * a = new Element(p.first);
        auto * b = new Element(p.second);
        disconnect();
        for (auto el : neighbors) {
            a->connectIfNeighboring(*el);
            b->connectIfNeighboring(*el);
        }
        a->connectIfNeighboring(*b);
        delete this;
        return std::make_pair(a,b);
    }

    
    
    void disconnect() {
        for(auto el : neighbors) {
            el->neighbors.erase(this);
        }
    }
    
    void connectIfNeighboring(Element& neighbor) {
        assert(&neighbor != this);
        using namespace std;
        if(getBounds().touches(neighbor.getBounds())) {
            neighbors.insert(&neighbor);
            neighbor.neighbors.insert(this);
        }
    }
    
};

#endif /* Element_h */
