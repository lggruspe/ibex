#include "generate.h"
#include <iostream>
#include <iterator>
#include <map>
#include <set>

void headers()
{
  std::cout <<
    "#include <iostream>\n"
    "#include <string>\n"
    "#include <utility>\n"
    "#include <vector>\n"
    << std::endl;
}

void transitions(int q, const std::map<char, int>& trans)
{
  std::map<int, std::set<char>> dests;   // invert transition
  for (auto it = trans.begin(); it != trans.end(); ++it) {
    dests[it->second].insert(it->first);
  }

  for (auto it = dests.begin(); it != dests.end(); ++it) {
    std::cout << "\tif (c == '" << *(it->second.begin()) << "'";
    for (auto jt = std::next(it->second.begin()); jt != it->second.end();
        ++jt) {
      std::cout << " || c == '" << *jt << "'";
    }
    std::cout << ") {\n";
    std::cout << "\t\tgoto s" << it->first << ";\n";
    std::cout << "\t}\n";
  }
}

void state(const Dfa& dfa, int q)
{
  std::cout <<
    "s" << q << ":\n"
    "\tstd::cin.get(c);\n"
    "\tlexeme += c;\n";

  if (dfa.accept.find(q) != dfa.accept.end()) {
    std::cout << 
      "\tcheckpoint.clear();\n"
      "\taccept = true;\n";
  }

  std::cout <<
    "\tcheckpoint.push_back(c);\n";

  const std::map<char, int>& trans = dfa.delta.at(q);
  transitions(q, trans);

  std::cout << "\tgoto se;\n" << std::endl;
}

void generate(const std::string& name, const Dfa& dfa)
{
  // TODO check if name is an identifier?
  headers();

  std::cout <<
    "std::pair<bool, std::string> " << name << "()\n"
    "{\n"
    "\tchar c;\n"
    "\tstd::vector<char> checkpoint;\n"
    "\tstd::string lexeme;\n"
    "\tbool accept = false;\n"
    "\tgoto s" << dfa.start << ";\n"
    << std::endl;

  for (auto it = dfa.delta.begin(); it != dfa.delta.end(); ++it) {
    state(dfa, it->first);
  }

  std::cout <<
    "se:\n"
    "\twhile (!checkpoint.empty()) {\n"
    "\t\tc = checkpoint.back();\n"
    "\t\tcheckpoint.pop_back();\n"
    "\t\tstd::cin.putback(c);\n"
    "\t\tlexeme.pop_back();\n"
    "\t}\n\n"
    "\treturn std::pair<bool, std::string>(accept, lexeme);\n"
    "}\n"
    << std::endl;
}

void extra(const std::string& name)
{
  std::cout <<
    "int main()\n"
    "{\n"
    "\tstd::pair<bool, std::string> res = " << name << "();\n"
    "\tstd::cout << res.first << ' ' << res.second << std::endl;\n"
    "}\n"
    << std::endl;
}
