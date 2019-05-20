#pragma once
#include "dfa.h"
#include <iostream>
#include <string>

namespace automata 
{
void print(const Dfa& dfa)
{
    std::cout << "start: " << dfa.start << std::endl;
    std::cout << "accept: ";
    for (auto f: dfa.accept) {
        std::cout << f << " ";
    }
    std::cout << std::endl;
    std::cout << "delta: " << std::endl;
    for (auto it = dfa.delta.begin(); it != dfa.delta.end(); ++it) {
        int q = it->first;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
            std::string a = jt->first;
            int r = jt->second;
            std::cout << "d(" << q << ", " << a << ") = " << r << std::endl;
        }
    }
    std::cout << std::endl;
}

}
