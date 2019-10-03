#pragma once
#include "distree/distree.h"

namespace rnd
{

using SymbolRange = distree::Interval;

using Alphabet = distree::DisSet;

bool overlaps(const distree::Interval&, const distree::Interval&);

bool is_empty(const SymbolRange&);

}
