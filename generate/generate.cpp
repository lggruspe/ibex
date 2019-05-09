#include "generate.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>

void transitions(std::ofstream& out, const std::map<std::string, int>& trans)
{
  std::map<int, std::set<std::string>> dests;   // invert transition
  for (auto it = trans.begin(); it != trans.end(); ++it) {
    dests[it->second].insert(it->first);
  }

  for (auto it = dests.begin(); it != dests.end(); ++it) {
    out << "\tif (cat == \"" << *(it->second.begin()) << "\"";
    for (auto jt = std::next(it->second.begin()); jt != it->second.end();
        ++jt) {
      out << " || cat == \"" << *jt << "\"";
    }
    out << ") {\n";
    out << "\t\tgoto s" << it->first << ";\n";
    out << "\t}\n";
  }
}

void state(std::ofstream& out, const automata::Dfa& dfa, int q)
{
  out <<
    "s" << q << ":\n"
    "\tstd::cin.get(c);\n"
    "\tlexeme += c;\n";

  if (dfa.accept.find(q) != dfa.accept.end()) {
    out << 
      "\tcheckpoint.clear();\n"
      "\taccept = true;\n";
  }

  out <<
    "\tcheckpoint.push_back(c);\n"
    "\tcat = category(c);\n";

  const std::map<std::string, int>& trans = dfa.delta.at(q);
  transitions(out, trans);

  out << "\tgoto se;\n" << std::endl;
}

void generate(std::ofstream& out, const std::string& name, const automata::Dfa& dfa)
{
  // TODO check if name is an identifier?
  out <<
    "#include \"category.h\"\n"
    "#include <iostream>\n"
    "#include <string>\n"
    "#include <utility>\n"
    "#include <vector>\n"
    << std::endl;


  out <<
    "std::pair<bool, std::string> " << name << "()\n"
    "{\n"
    "\tchar c;\n"
    "\tstd::string cat;\n"      // TODO new
    "\tstd::vector<char> checkpoint;\n"
    "\tstd::string lexeme;\n"
    "\tbool accept = false;\n"
    "\tgoto s" << dfa.start << ";\n"
    << std::endl;

  for (auto it = dfa.delta.begin(); it != dfa.delta.end(); ++it) {
    state(out, dfa, it->first);
  }

  out <<
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

void extra(std::ofstream& out, const std::string& name)
{
  out <<
    "int main()\n"
    "{\n"
    "\tstd::pair<bool, std::string> res = " << name << "();\n"
    "\tstd::cout << res.first << ' ' << res.second << std::endl;\n"
    "}\n"
    << std::endl;
}
