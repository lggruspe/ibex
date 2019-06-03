#pragma once
#include <boost/icl/split_interval_set.hpp>

using Alphabet = typename boost::icl::split_interval_set<char>;

void insert(Alphabet&, char, char);
int categorize(const Alphabet&, char);
