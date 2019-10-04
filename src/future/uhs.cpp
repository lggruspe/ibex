#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>

template <class T>
class UniqueHandleSet {
    using Map = std::map<T, int>;
    using Vector = std::vector<typename Map::iterator>;
    Map map;
    Vector vector;

public:
    UniqueHandleSet() {}

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

int main()
{
    std::set<int> s = {1, 2, 3, 5};
    std::set<int> t = {2, 11};
    std::set<int> u = s;

    UniqueHandleSet<std::set<int>> H;
    std::cout << H.index(s) << std::endl;
    std::cout << H.index(t) << std::endl;
    std::cout << H.index(u) << std::endl;

    /*
    std::cout << H.index(s) << std::endl;
    std::cout << H.index(t) << std::endl;
    std::cout << H.index(u) << std::endl;

    for (const auto& a: H.value(0)) {
        std::cout << a << " ";
    }
    std::cout << std::endl;

    for (const auto& a: H.value(1)) {
        std::cout << a << " ";
    }
    std::cout << std::endl;
    */

    std::cout << "contains\n";
    std::cout << H.contains(s) << std::endl;
    std::cout << H.contains(t) << std::endl;
    std::cout << H.contains(u) << std::endl;
}
