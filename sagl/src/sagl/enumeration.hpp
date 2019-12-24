#pragma once
#include <algorithm>
#include <map>

template <class T>
class Enumeration {
    std::map<int, T> values;
    std::map<T, int> indices;

    int next_index() const {
        if (values.empty()) {
            return 0;
        }
        return values.rbegin()->first + 1;
    }

    struct PredicateWrapper {
        PredicateWrapper(bool pred(const T& t)) : predicate(pred) {}

        bool operator()(const std::pair<T, int>& pair) const {
            return predicate(pair.first);
        }

    private:
        bool (*predicate)(const T& t);
    };

public:
    using iterator = typename std::map<int, T>::iterator;
    using const_iterator = typename std::map<int, T>::const_iterator;

    iterator begin() { return values.begin(); }
    iterator end() { return values.end(); }
    const_iterator begin() const { return values.begin(); }
    const_iterator end() const { return values.end(); }

    typename std::map<T, int>::const_iterator ibegin() const {
        return indices.begin();
    }

    typename std::map<T, int>::const_iterator iend() const {
        return indices.end();
    }

    const T& value(int ind) const {
        return values.at(ind);
    }

    int index(const T& val) const {
        return indices.at(val);
    }

    void clear() {
        indices.clear();
        values.clear();
    }

    int insert(const T& val) {
        auto it = indices.find(val);
        if (it != indices.end()) {
            return it->second;
        }

        int ind = next_index();
        values[ind] = val;
        indices[val] = ind;
        return ind;
    }

    typename std::map<T, int>::const_iterator
    find_if(bool pred(const T& t)) {
        return std::find_if(indices.begin(), indices.end(),
                PredicateWrapper(pred));
    }

    bool has_value(const T& val) const {
        return indices.find(val) != indices.end();
    }

    bool has_index(int ind) const {
        return values.find(ind) != values.end();
    }
};
