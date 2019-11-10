#pragma once
#include <map>
#include <utility>
#include <vector>

template <class T>
class HandleSet {
    using Map = std::map<T, int>;
    using Vector = std::vector<typename Map::iterator>;
    Map map;
    Vector vector;

public:
    std::pair<int, bool> index(const T& t)
    {
        int handle = vector.size();
        auto [it, ok] = map.insert({t, handle});
        if (!ok) {
            return {it->second, false};
        }
        vector.push_back(it);
        return {handle, true};
    }

    int index_only(const T& t)
    {
        auto [id, _] = index(t);
        return id;
    }

    const T& value(int ind) const
    {
        return vector[ind]->first;
    }

    auto begin() const
    {
        return map.begin();
    }

    auto end() const
    {
        return map.end();
    }

    bool contains(const T& val) const
    {
        return map.find(val) != map.end();
    }
};
