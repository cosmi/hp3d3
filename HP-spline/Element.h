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
    const CellId<DIMS>& getBounds() const {
        return bounds;
    }
    auto& getNeighbors() const {
        return neighbors;
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
        using namespace std;
        if(getBounds().isNeighboring(neighbor.getBounds())) {
//            cout << "Nei" << getBounds() << " " << neighbor.getBounds() << endl;
//            cout << getBounds().getOverlap(neighbor.getBounds()) << endl;
            neighbors.insert(&neighbor);
            neighbor.neighbors.insert(this);
        } else {
//            cout << "NotNei" << getBounds() << " " << neighbor.getBounds() << endl;
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
