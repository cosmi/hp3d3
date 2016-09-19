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
#include "SVGRenderer.h"
#include <stack>

template<int DIMS>
void ensureNeighborDegree(Mesh<DIMS> &mesh, Element<DIMS> *el) {
    using namespace std;
    std::vector<Element<DIMS>*> neighbors(el->getNeighbors().begin(), el->getNeighbors().end());
    while(!neighbors.empty()) {
        auto nei = neighbors.back();
        neighbors.pop_back();
//        cout << nei->getBounds() << " " << el->getBounds() << endl;
        auto overlap = nei->getBounds().getOverlap(el->getBounds());
        auto side = el->getBounds().getAdjacentSide(nei->getBounds());
        FOR(i, DIMS) {
            if(overlap.getLength(i) > 0 && side.getLength(i) > 2 * overlap.getLength(i)) {
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
                        auto children = mesh.split(el, i);
                        for(auto ch : {children.first, children.second} ) {
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
        assert(isQuadLikeMesh(mesh));
    }
}



template<int DIMS>
bool isQuadLikeMesh(const Mesh<DIMS>& mesh, bool checkTauRule = false) {
    using namespace std;
    // for each neighboring element pair, one of them is equal or lower level than another in all common dimensions
    for(auto el : mesh.getElements()) {
        for(auto nei : el->getNeighbors()) {
            auto overlap = nei->getBounds().getOverlap(el->getBounds());
            auto elSide = el->getBounds().getAdjacentSide(nei->getBounds());
            auto neiSide = nei->getBounds().getAdjacentSide(el->getBounds());
            if(neiSide == elSide) continue;
            if(overlap != neiSide && overlap != elSide) {
                // they are of uncomparable level
                cerr << "uncomparable level" << endl;
                return false;
            }
            auto largeSide = (overlap == neiSide)?elSide:neiSide;
            
            dim_t multiplier = 0;
            FOR(i, DIMS) {
                assert((largeSide.getLength(i) != 0) == (neiSide.getLength(i) != 0));
                if(largeSide.getLength(i) != 0) {
                    dim_t newMultiplier = largeSide.getLength(i)/overlap.getLength(i);
                    if(largeSide.getLength(i) != overlap.getLength(i) * newMultiplier) {
                        // non-integer division
                        cerr << "non integer division" << endl;
                        return false;
                    }
                    if(multiplier != 0 && newMultiplier != multiplier) {
                        // different level differences for various dimensions
                        cerr << "different level differences" << endl;
                        return false;
                    }
                    multiplier = newMultiplier;
                }
            }
            if(checkTauRule && multiplier != 2) {
                
                cerr << "Does not match tauRule " << el->getBounds() << nei->getBounds() << endl;
                return false;
            }
            if(!isPowerOfTwo(multiplier)) {
                // the difference of levels cannot be fixed by quad division
                cerr << "cannot be fixed by quad division: " << multiplier << " " << elSide << neiSide << overlap << endl;
                return false;
            }
        }
    }
    return true;
}

template<int DIMS>
bool shouldElementBeSplitForTauRule(const Element<DIMS>& el) {
    for(auto nei: el.getNeighbors()) {
        auto overlap = nei->getBounds().getOverlap(el.getBounds());
        auto elSide = el.getBounds().getAdjacentSide(nei->getBounds());
        auto neiSide = nei->getBounds().getAdjacentSide(el.getBounds());
        if(overlap == neiSide) {
            FOR(i, DIMS) {
                if(elSide.getLength(i) > 2*neiSide.getLength(i)) return true;
            }
        } else assert(overlap == elSide);
    }
    return false;
}

template<int DIMS>
void ensureTauRuleForQuadLikeMesh(Mesh<DIMS>& mesh) {
    assert(isQuadLikeMesh(mesh));
    std::set<Element<DIMS>*> S;
    for(auto el : mesh.getElements()) {
        S.insert(el);
    }
    while(!S.empty()) {
        auto el = *S.begin();
        S.erase(S.begin());
        if(shouldElementBeSplitForTauRule(*el)) {
            for(auto nei: el->getNeighbors()) {
                S.insert(nei);
            }
            mesh.splitInAllDims(el);
        }
    }
}

template<int DIMS>
bool isQuadLikeTauRuleMesh(const Mesh<DIMS>& mesh) {
    return isQuadLikeMesh(mesh, true);
}


#endif /* meshBuilders_h */
