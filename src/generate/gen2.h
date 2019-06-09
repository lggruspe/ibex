#pragma once
#include "dfa/dfa.h"
#include <boost/icl/split_interval_set.hpp>
#include <iostream>
#include <string>
#include <vector>


namespace gen2
{

using Alphabet = typename boost::icl::split_interval_set<char>;

void includes(std::ostream&, bool);
void tokens(std::ostream&, const std::vector<std::string>&);
void token_printer(std::ostream&, const std::vector<std::string>&);
void base_scanner(std::ostream&);
void scanner(std::ostream&, const std::string&, const automata::Dfa&,
        const Alphabet&);
void scanner_collection(std::ostream&, const std::vector<std::string>&);
}
