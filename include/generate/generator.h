#pragma once
#include "regex2/regex2.h"
#include <iostream>
#include <map>
#include <string>

namespace gen2
{
void generate(std::ostream&, const std::map<std::string, regex2::Expr>&);
}
