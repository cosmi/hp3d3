//
//  CellId.h
//  HP3d
//
//  Created by Marcin on 05.06.2015.
//  Copyright (c) 2015 Marcin. All rights reserved.
//

#ifndef __HP3d__CellId__
#define __HP3d__CellId__
#include <algorithm>
#include <list>
#include <set>

#include <unordered_set>
#include "defs.h"
#include "Point.h"

template <int DIMS>
class CellId {
    using PointBase = ::PointBase<DIMS>;
    using Point = ::Point<DIMS>;
    using PointDifference = ::PointDifference<DIMS>;
    Point from, to;
    
public:
    CellId(const Point& from, const Point& to): from(from), to(to){}
    CellId(const Point& from, const PointDifference& size): from(from), to(from+size){}
    CellId(std::initializer_list<dim_t> from, std::initializer_list<dim_t> to) : from(from), to(to) {}
    static CellId unit() {
        return CellId(Point::origin(), PointDifference::cube(1));
    }
    static CellId cube(dim_t size) {
        return CellId(Point::origin(), PointDifference::cube(size));
    }
    static CellId null() {
        return CellId(Point::origin(), Point::origin());
    }
    template <int NEW_DIMS>
    CellId<NEW_DIMS> redimensionalize() const {
        CellId<NEW_DIMS> id = CellId<NEW_DIMS>::null();
        FOR(i, NEW_DIMS) {
            if(i<DIMS) {
                id = id.withDimension(i, this->getFrom()[i], this->getTo()[i]);
            }
        }
        return id;
    }
    
    CellId<DIMS-1> removeDimension(size_t dim) const {
        return CellId<DIMS-1>(from.removeDimension(dim), to.removeDimension(dim));
    }
    
    bool isValid() const {
        FOR(i, DIMS) {
            if(from[i] > to[i]) return false;
        }
        return true;
    }
    
    const Point& getFrom() const {
        return from;
    }
    const Point& getTo() const {
        return to;
    }
    PointDifference getSize() const {
        return to - from;
    }
    dim_t getSide() const {
        return getSize().getSide();
    }
    CellId operator+(const PointDifference & pd) const {
        return CellId(getFrom() + pd, getTo() + pd);
    }
    CellId operator-(const PointDifference & pd) const {
        return CellId(getFrom() - pd, getTo() - pd);
    }
    bool isCube() const {
        auto size = getSize();
        return size.isCube();
    }
    bool isAligned() const {
        auto size = getSize();
        auto & from = getFrom();
        FOR(i, DIMS) {
            dim_t p = size[i]-1;
            if(from[i] & p) return false;
        }
        return true;
    }
    bool operator==(const CellId& cid) const {
        return getFrom() == cid.getFrom() && getTo() == cid.getTo();
    }
    bool operator!=(const CellId& cid) const {
        return getFrom() != cid.getFrom() || getTo() != cid.getTo();
    }
    
    CellId getChild(mask_t id) const {
        auto pd = getSize()>>1;
        auto bmpd = pd.selectDimsByBitMask(id);
        return CellId(getFrom() + bmpd, pd);
    }
    
    CellId flattenDimsByBitMask(mask_t mask, mask_t side) const {
        auto size = getSize();
        return CellId(getFrom() + size.selectDimsByBitMask(side & mask), size.selectDimsByBitMask(~mask));
    }
    
    constexpr size_t getChildrenCount() const {
        return 1<<DIMS;
    }
    
    
    
    CellId move(int direction, int distance) const {
        auto size = getSize();
        auto & from = getFrom();
        auto diff = size.selectDim(direction) * distance;
        return CellId(from+diff, size);
    }
    
    CellId moveDimension(size_t dimension, dim_t distance) const {
        return *this + PointDifference::singleDimension(dimension, distance);
    }
    CellId withDimension(size_t dim, dim_t from, dim_t to) const {
        return CellId(getFrom().withDimension(dim, from), getTo().withDimension(dim, to));
    }
    
    bool covers(const CellId& cid) const {
        return getFrom().hasRelation(cid.getFrom(), PointRelation::LESS_OR_EQ)
        && getTo().hasRelation(cid.getTo(), PointRelation::MORE_OR_EQ);
    }
    
    bool stronglyOverlaps(const CellId& cid) const {
        return getNeighborhoodLevel(cid) == DIMS;
    }
    /*
     * Has at least common boundary?
     */
    bool touches(const CellId& cid) const {
        return getNeighborhoodLevel(cid) >= 0;
//        bool t = true;
//        FOR(i, DIMS) {
//            if(to[i]<cid.getFrom()[i] || from[i] > cid.getTo()[i]) t = false;
//        }
//        int lvl = getNeighborhoodLevel(cid);
//        if((lvl >= 0) != t) {
//            std::cout << ">>" << *this << ' ' << cid << " " << lvl << std::endl;
//        }
//        return t;
    }
    
    
    /* 
     * Has common side
     */
    bool isNeighboring(const CellId& cid) const {
        return getNeighborhoodLevel(cid) == DIMS-1;
    }
    
    /*
     * -1 is not touching
     * 0 is touching with corners
     * DIMS is overlapping (or equal)
     */
    int getNeighborhoodLevel(const CellId& cid) const {
        int overlaps = 0;
        int touches = 0;
        FOR(i, DIMS) {
            if(to[i] > cid.getFrom()[i] && from[i] < cid.getTo()[i]) {
                overlaps++;
            } else if(to[i] == cid.getFrom()[i] || from[i] == cid.getTo()[i]) {
                touches++;
            } else {
                return -1;
            }
        }
//        std::cout << ">>" << *this << ' ' << cid << " " << overlaps << ' '<< touches<< std::endl;
        assert(touches + overlaps == DIMS);
//        if(touches+overlaps < DIMS) return -1;
        return overlaps;
    }
    
    bool touchesInteriorOf(const CellId& cid) const {
        return overlaps(cid);
    }
    
    bool overlaps(const CellId& cid) const {
        FOR(i, DIMS) {
            if(getTo()[i] <= cid.getFrom()[i] || getFrom()[i] >= cid.getTo()[i]) return false;
        }
        return true;
    }
    
    bool isSideOf(const CellId& cid) const {
        int eq = 0;
        int touch = 0;
        FOR(i, DIMS) {
            if(getFrom()[i] == getTo()[i]) {
                if(getFrom()[i] == cid.getFrom()[i] || getFrom()[i] == cid.getTo()[i]) touch++;
            } else {
                if(getFrom()[i] == cid.getFrom()[i] && getTo()[i] == cid.getTo()[i]) eq++;
            }
        }
        return (eq + touch == DIMS) && touch == 1;
    }
    
    bool isOnFrontalSideOf(const CellId& cid) const {
        int eq = 0;
        int touch = 0;
        FOR(i, DIMS) {
            if(getFrom()[i] == getTo()[i]) {
                if(getFrom()[i] == cid.getFrom()[i] /*|| getFrom()[i] == cid.getTo()[i]*/) touch++;
            } else {
                if(getFrom()[i] >= cid.getFrom()[i] || getTo()[i] <= cid.getTo()[i]) eq++;
                eq++;
            }
        }
        return (touch >= 1);
    }
    
    
    /* Bounds methods */
    
    static CellId getBounds(const CellId& a, const CellId& b) {
        return CellId(Point(PointBase::selectMinDims(a.getFrom(),b.getFrom())), Point(PointBase::selectMaxDims(a.getTo(),b.getTo())));
    }
    
    template <typename Iterator>
    static CellId getBounds(Iterator begin, const Iterator& end) {
        assert(begin != end);
        CellId res = *begin;
        while(++begin != end) {
            res = CellId::getBounds(res, *begin);
        }
        return res;
    }
    
    
    
    CellId getHalf() const {
        auto size = getSize();
        size_t splittingDim = size.getLongestDim();
        assert(size[splittingDim]%2 == (dim_t)0);
        auto nsize = PointDifference(size.replaceDim(splittingDim, size[splittingDim]/2));
        return CellId(getFrom(), nsize);
    }
    
    CellId getHalf(size_t dim) const {
        dim_t size = getSize()[dim];
        assert(size > 1 && size%2 == 0);
        PointDifference nsize(getSize().replaceDim(dim, size/2));
        return CellId(getFrom(), nsize);
    }
    CellId getSecondHalf(size_t dim) const {
        dim_t size = getSize()[dim];
        assert(size > 1 && size%2 == 0);
        PointDifference nsize(getSize().replaceDim(dim, size/2));
        return CellId(Point(getFrom().replaceDim(dim, getFrom()[dim]+size/2)), nsize);
    }
    
    CellId getOverlap(const CellId& c2) const {
        CellId cid = *this;
        FOR(i, DIMS) {
            cid.from.set(i, std::max(from[i], c2.from[i]));
            cid.to.set(i, std::min(to[i], c2.to[i]));
            assert(cid.from[i] <= cid.to[i]);
        }
        return cid;
    }
    
    CellId getAdjacentSide(const CellId& c2) const {
        CellId cid = *this;
        FOR(i, DIMS) {
            if(from[i] == c2.to[i]) {
                cid.to.set(i, c2.to[i]);
            }
            if(to[i] == c2.from[i]) {
                cid.from.set(i, c2.from[i]);
            }
        }
        return cid;
    }
    
    dim_t getLength(int dim) const {
        return to[dim] - from[dim];
    }
    
    /* Node methods */
    
    size_t countNonZeroDims() const {
        size_t ret = 0;
        FOR(i, DIMS) {
            if(from[i] != to[i]) ret++;
        }
        return ret;
    }
    size_t getDimensionality() const {
        return countNonZeroDims();
    }
    bool isFullyDimensional() const {
        return getDimensionality() == DIMS;
    }
    
    size_t countDimsOnBounds(const CellId& bounds) const {
        size_t ret = 0;
        FOR(i, DIMS) if(from[i] == to[i]) {
            auto v = from[i];
            if(bounds.getFrom()[i] == v || bounds.getTo()[i] == v) {
                ret++;
            }
        }
        return ret;
    }
    
    bool isNodeOnCell(const CellId& cid) const {
        FOR(i, DIMS) if(from[i] != to[i]) {
            if(cid.getFrom()[i] != from[i] || cid.getTo()[i] != to[i]) return false;
        } else {
            
            //            assert(!(cid.getFrom()[i] != from[i] && cid.getTo()[i] != from[i])); // TODO: for now, disallow false here
            if(cid.getFrom()[i] != from[i] && cid.getTo()[i] != from[i]) return false;
        }
        return true;
    }
    
    CellId getReducedDimId(int dim, bool onTo) const {
        if(onTo) {
            return CellId(Point(from.replaceDim(dim, to[dim])), to);
        } else {
            return CellId(from, Point(to.replaceDim(dim, from[dim])));
        }
    }
    
    using IdList = std::list<CellId>;
    IdList getLowerDimensionalityIds() const {
        IdList list;
        FOR(i, DIMS) if(from[i] != to[i]) {
            list.push_back(getReducedDimId(i, false));
            list.push_back(getReducedDimId(i, true));
        }
        return list;
    }
    
    std::pair<CellId, CellId> split(int dim) const {
        std::pair<CellId, CellId> p(*this, *this);
        
        dim_t div = (to[dim]-from[dim])/2 + from[dim];
        p.first.to.set(dim, div);
        p.second.from.set(dim, div);
        return p;
    }
    
private:
    
    void getSameSizeNeighborsHelper(IdList& list, int dim) const {
        
        if(dim == DIMS) {
            list.push_back(*this);
        } else {
            for(int i = -1; i<=1; i++) {
                auto id = move(dim, i);
                id.getSameSizeNeighborsHelper(list, dim+1);
            }
        }
    }
public:
    
    IdList getSameSizeNeighbors() const {
        IdList list;
        getSameSizeNeighborsHelper(list, 0);
        return list;
    }
    
    using IdSet = std::unordered_set<CellId>;
    IdSet getLowerDimensionalityBounds(size_t target) const {
        IdSet ret;
        ret.insert(*this);
        
        for(size_t dims = getDimensionality(); dims > target; dims--) {
            IdSet ret2;
            for(auto & id: ret) {
                auto l = id.getLowerDimensionalityIds();
                ret2.insert(l.begin(), l.end());
            }
            swap(ret, ret2);
        }
        
        return ret;
    }
    
    CellId getAlignedAncestor(int bits) const {
        auto size = getSize();
        mask_t dimmask = size.getNonZeroMask();
        auto newpt = from.resetBitsInMaskedDims(bits, dimmask);
        auto newsize = size.setNonZeroDimsTo(1<<bits);
        return CellId(newpt, newsize);
    }
    
    CellId getAlignedParent() const {
        auto size = getSize();
        auto nsize = size << 1;
        mask_t dimmask = size.getNonZeroMask();
        int bits = nsize.countZeroBits();
        auto pt = getFrom().resetBitsInMaskedDims(bits, dimmask);
        return CellId(pt, nsize);
    }
    
    static CellId intersection(const CellId& a, const CellId& b)  {
        return CellId(Point(PointBase::selectMaxDims(a.getFrom(), b.getFrom())),
                      Point(PointBase::selectMinDims(a.getTo(), b.getTo())));
    }
    
    size_t hash() const {
        size_t ret = 0;
        const size_t PRIME = 920419823;
        ret = getFrom().hash() + PRIME * getTo().hash();
        return ret;
    }
    
    static CellId getForSize(std::initializer_list<dim_t> list) {
        return CellId(Point::origin(), Point(list));
    }
};


template <int DIMS>
std::ostream& operator<<(std::ostream& os,
                         const CellId<DIMS>& id) {
    //    return os << "C[" << id.getFrom() << ';'<< id.getTo() << ']';
    
    os << "C[";
    bool first = true;
    FOR(i, DIMS) {
        if(!first) os << ';';
        os << id.getFrom()[i] << ".." << id.getTo()[i];
        first = false;
    }
    return os << ']';
}

namespace std {
    template<int DIMS>
    struct hash<CellId<DIMS> >: public unary_function<CellId<DIMS>, size_t> {
        
        size_t operator()(const CellId<DIMS>& id) const {
            return id.hash();
        }
        hash() {}
    };
}


#endif /* defined(__HP3d__CellId__) */
