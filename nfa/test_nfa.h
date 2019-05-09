#pragma once
#include "nfa.h"
#include <iostream>

namespace nfa {
    void print(const Nfa& N)
    {
      std::cout << "start: " << N.start << std::endl;
      std::cout << "accept: " << N.accept << std::endl;
      std::cout << "symbols: ";
      for (auto a: N.symbols) {
        std::cout << a << " ";
      }
      std::cout << std::endl;
      std::cout << "transitions:" << std::endl;

      for (auto it = N.delta.begin(); it != N.delta.end(); ++it) {
        int q = it->first;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
          std::string a = jt->first;
          for (auto r: jt->second) {
            std::cout << "(" << q << ", " << a << ", " << r << ")\n";
          }
        }
      }
      std::cout << std::endl << std::endl;
    }
}
