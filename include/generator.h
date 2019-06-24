#pragma once
#include <rnd/regex.h>
#include <iostream>
#include <map>
#include <string>

namespace gen2
{
void generate(std::ostream&, const std::map<std::string, regex::Expr>&);
}
