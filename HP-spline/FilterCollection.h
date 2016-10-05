//
//  FilterCollection.h
//  HP-spline
//
//  Created by Marcin Skotniczny on 23.09.2016.
//  Copyright © 2016 Marcin Skotniczny. All rights reserved.
//

#ifndef FilterCollection_h
#define FilterCollection_h
#include <vector>

template<class Filter, class Collection>
class FilterCollection {
public:
    using value_type = typename Collection::value_type;
private:
    using iterator_type = typename Collection::const_iterator;
    Filter filter;
    const Collection& coll;
    
public:
    class const_iterator {
        const FilterCollection& outer;
        iterator_type it;
        void jump() {
            while (it!=outer.coll.end() && !outer.filter(*it) ) {
                it++;
            }
        }
    public:
        const_iterator(const iterator_type& it, const FilterCollection& outer ):it(it), outer(outer) {
            jump();
        }
        const auto& operator*() const {
            return *it;
        }
        const_iterator& operator++() {
            ++it;
            jump();
            return *this;
        }
        bool operator==(const const_iterator& cit) const {
            return it == cit.it;
        }
        bool operator!=(const const_iterator& cit) const {
            return it != cit.it;
        }
    };
    
    FilterCollection(const Collection& coll, const Filter& filter): coll(coll), filter(filter) {}
    const_iterator begin() const {
        return const_iterator(coll.begin(), *this);
    }
    const_iterator end() const {
        return const_iterator(coll.end(), *this);
    }
    
    size_t size() const {
        size_t count = 0;
        for(auto& el : *this) {
            count++;
        }
        return count;
    }
    operator std::vector<value_type>() const {
        std::vector<value_type> result;
        for(auto& it : *this) {
            result.push_back(it);
        }
        return result;
    }
};

template<class Filter, class Collection>
FilterCollection<Filter, Collection> filter(const Collection& coll, const Filter& filter) {
    return FilterCollection<Filter, Collection>(coll, filter);
}


#endif /* FilterCollection_h */
