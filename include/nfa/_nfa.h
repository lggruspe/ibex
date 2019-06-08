#pragma once
#include "nfa.h"
#include <boost/icl/split_interval_set.hpp>

namespace automata
{

Nfa symbol(char);
Nfa symbol(boost::icl::interval<char>::type);
Nfa alternate(Nfa&, const Nfa&);
Nfa concatenate(Nfa&, const Nfa&);
Nfa closure(Nfa&);

}
