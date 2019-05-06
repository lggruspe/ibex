#include "test_nfa.h"
#include <iostream>

void print_nfa(const Nfa& nfa)
{
  std::cout << "start: " << nfa.start << std::endl;
  std::cout << "accept: " << nfa.accept << std::endl;
  std::cout << "symbols: ";
  for (auto a: nfa.symbols) {
    std::cout << a << " ";
  }
  std::cout << std::endl;
  std::cout << "transitions:" << std::endl;

  for (auto it = nfa.delta.begin(); it != nfa.delta.end(); ++it) {
    int q = it->first;
    for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
      char a = jt->first;
      for (auto r: jt->second) {
        std::cout << "(" << q << ", " << a << ", " << r << ")\n";
      }
    }
  }
  std::cout << std::endl << std::endl;
}
