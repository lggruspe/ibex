#pragma once
#include "../dfa/dfa.h"
#include <fstream>
#include <string>

void generate(std::ofstream& out, const std::string&, const automata::Dfa&);
void extra(std::ofstream& out, const std::string&);
