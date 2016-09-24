//
//  Mesh.h
//  HP-spline
//
//  Created by Marcin on 27.04.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef Mesh_h
#define Mesh_h
#include <set>
#include <list>
#include <stack>
#include "Element.h"
#include "CellId.h"
#include "QuadTree.h"

template<int DIMS>
class Mesh {
public:
    using Element = ::Element<DIMS>;

protected:
    std::unordered_set<Element*> elements;
    QuadTree<DIMS, Element> elementsTree;
    
public:
    Mesh(CellId<DIMS> bounds) : elementsTree(bounds) {
        addElement(new Element(bounds));
    }
    ~Mesh() {
        for(auto el : elements) delete el;
    }
    
    const CellId<DIMS>& getBounds() const {
        return elementsTree.getBounds();
    }
    auto& getElements() const {
        return elements;
    }
    
    void removeElement(Element* el) {
        elements.erase(el);
        elementsTree.removeElement(el);
    }
    
    void addElement(Element* el) {
        elements.insert(el);
        elementsTree.addElement(el);
    }
    
    std::pair<Element*, Element*> split(Element* e, int dim) {
        removeElement(e);
        auto p = e->split(dim);
        addElement(p.first);
        addElement(p.second);
        return p;
    }
    
    std::list<Element*> splitInAllDims(Element* e) {
        removeElement(e);
        std::list<Element*> ret(1,e);
        FOR(dim, DIMS) {
            for(auto it = ret.begin(); it!=ret.end();) {
                auto p = (*it)->split(dim);
                ret.push_front(p.first);
                ret.push_front(p.second);
                it = ret.erase(it);
            }
        }
        assert(ret.size() == 1<<DIMS);
        for(auto el: ret) {
            addElement(el);
        }
        return ret;
    }
    
    std::list<Element*> getAllElementsInBounds(const CellId<DIMS>& bounds, Element* hint) const {
        assert(elements.count(hint) == 1);
        assert(bounds.overlaps(hint->getBounds()));
        using namespace std;
        
        list<Element*> ret;
        ret.push_front(hint);
        
        unordered_set<Element*> visited;
        visited.insert(hint);
        stack<Element*> S;
        S.push(hint);
        
        while(!S.empty()) {
            Element* el = S.top(); S.pop();
            assert(bounds.overlaps(el->getBounds()));
            for(auto nei: el->getCornerNeighbors()) if(visited.count(nei) == 0) {
                visited.insert(nei);
                if(bounds.overlaps(nei->getBounds())) {
                    S.push(nei);
                    ret.push_front(nei);
                }
            }
        }
        return ret;
    }
    
    auto& getQuadTree() const {
        return elementsTree;
    }
};


#endif /* Mesh_h */
