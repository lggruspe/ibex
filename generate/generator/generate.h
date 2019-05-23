#pragma once
#include "dfa.h"
#include <fstream>
#include <string>

void generate(std::ofstream&, const std::string&, const automata::Dfa&);
void extra(std::ofstream&, const std::string&);
