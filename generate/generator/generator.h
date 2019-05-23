#pragma once
#include "dfa.h"
#include <iostream>
#include <map>
#include <string>

void generate_includes(std::ostream&, bool);
void generate_class(std::ostream&, const std::string&, const automata::Dfa&,
        const std::map<std::string, std::string>&,
        std::string::size_type);
