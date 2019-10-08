#include "handles.hpp"
#include <iostream>
#include <set>
#include <tuple>

int main()
{
    std::set<int> s = {1, 2, 3, 5};
    std::set<int> t = {2, 11};
    std::set<int> u = s;

    HandleSet<std::set<int>> H;
    auto [i, _] = H.index(s);
    std::cout << i << std::endl;
    std::tie(i, _) = H.index(t);
    std::cout << i << std::endl;
    std::tie(i, _) = H.index(u);
    std::cout << i << std::endl;

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
