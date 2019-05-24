#pragma once
#include "dfa.h"
#include <iostream>
#include <map>
#include <string>

void generate_includes(std::ostream&, bool);
void generate_base_scanner_class(std::ostream&, std::string::size_type);
void generate_scanner_class(std::ostream&, const std::string&, const automata::Dfa&,
        const std::map<std::string, std::string>&,
        std::string::size_type);
void generate_collection_class(std::ostream&, std::string::size_type);
