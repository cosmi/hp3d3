//
//  MapCollection.h
//  HP-spline
//
//  Created by Marcin Skotniczny on 24.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef MapCollection_h
#define MapCollection_h
#include <vector>

template<class Filter, class Collection>
class MapCollection {
public:
    using value_type = typename Collection::value_type;
private:
    using iterator_type = typename Collection::const_iterator;
    Filter filter;
    const Collection& coll;
    
    class const_iterator {
        const MapCollection& outer;
        iterator_type it;
    public:
        const_iterator(const iterator_type& it, const MapCollection& outer ):it(it), outer(outer) {
        }
        auto operator*() const {
            return outer.filter(*it);
        }
        const_iterator& operator++() {
            ++it;
            return *this;
        }
        bool operator==(const const_iterator& cit) const {
            return it == cit.it;
        }
        bool operator!=(const const_iterator& cit) const {
            return it != cit.it;
        }
    };
    
public:
    MapCollection(const Collection& coll, const Filter& filter): coll(coll), filter(filter) {}
    const_iterator begin() const {
        return const_iterator(coll.begin(), *this);
    }
    const_iterator end() const {
        return const_iterator(coll.end(), *this);
    }
    
    operator std::vector<value_type>() const {
        std::vector<value_type> result;
        for(auto& it : *this) {
            result.push_back(it);
        }
        return result;
    }
    
    auto size() const {
        return coll.size();
    }
};

template<class Filter, class Collection>
MapCollection<Filter, Collection> mapping(const Collection& coll, const Filter& filter) {
    return MapCollection<Filter, Collection>(coll, filter);
}



#endif /* MapCollection_h */
