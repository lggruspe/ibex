#include <vector>
#include <map>

template <class T>
class UniqueHandleSet {
    using Map = std::map<T, int>;
    using Vector = std::vector<typename Map::iterator>;
    Map map;
    Vector vector;

public:
    int index(const T& t)
    {
        auto [it, ok] = map.insert({t, vector.size()});
        if (!ok) {
            return it->second;
        }
        int handle = vector.size();
        vector.push_back(it);
        return handle;
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
