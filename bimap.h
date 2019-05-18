#pragma once
#include <algorithm>
#include <map>

template <class T, class U>
class Bimap {
    std::map<T, U> left;
    std::map<U, T> right;

public:
    const U& get(const T& a) const { 
        return left.at(a); 
    }
    const T& rget(const U& b) const { 
        return right.at(b); 
    }
    void set(const T& a, const U& b)
    {
        auto it = left.find(a);
        auto jt = right.find(b);
        if (it != left.end()) {
            left.erase(it);
        }
        if (jt != right.end()) {
            right.erase(jt);
        }
        left[a] = b;
        right[b] = a;
    }

    bool contains(const T& a) const { 
        return left.find(a) != left.end(); 
    }
    bool rcontains(const U& a) const { 
        return right.find(a) != right.end(); 
    }

    T next() const {
        return left.rbegin()->first + 1;    // doesn't always make sense
    }

    T insert(const U& b) {
        // TODO insert only if b not rcontained
        T a = next();
        set(a, b);
        return a;
    }
};
