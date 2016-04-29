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
    
    void split(Element* e, int dim) {
        removeElement(e);
        auto p = e->split(dim);
        addElement(p.first);
        addElement(p.second);
    }
    
    // Will autodestruct
    void splitInAllDims(Element* e) {
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
    }
    
};


#endif /* Mesh_h */
