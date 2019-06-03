#pragma once
#include <algorithm>
#include <map>
#include <vector>

template <class T>
class Enumeration {
    using size_type = typename std::vector<T>::size_type;
    std::map<T, size_type> indices;
    std::vector<T> values;
 
public:
    using iterator = typename std::map<T, size_type>::iterator;
    using const_iterator = typename std::map<T, size_type>::const_iterator;

    iterator begin() { return indices.begin(); }
    iterator end() { return indices.end(); }
    const_iterator begin() const { return indices.begin(); }
    const_iterator end() const { return indices.end(); }

    T& value(size_type ind) { return values[ind]; }
    const T& value(size_type ind) const { return values[ind]; }
    size_type index(const T& val) const { return indices.at(val); }

    void clear() {
        indices.clear();
        values.clear();
    }

    size_type insert(const T& val) {
        size_type ind = values.size();
        auto res = indices.insert(std::pair<T,size_type>(val, ind));
        if (res.second) {
            values.push_back((res.first)->first);
            return ind;
        } else {
            return (res.first)->second;
        }
    }

    bool has_value(const T& val) const {
        return indices.find(val) != indices.end();
    }

    bool has_index(size_type ind) const {
        return ind < values.size();
    }
};
