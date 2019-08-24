#include "alphabet.hpp"
#include "distree/distree.h"

namespace rnd
{

SymbolRange symbol_range(int start, int end)
{
    SymbolRange range;
    range.start = start;
    range.end = end;
    return range;
}

bool is_empty(const SymbolRange& range)
{
    return range.start == 0 && range.end == 0;
}

std::ostream& operator<<(std::ostream& out, const SymbolRange& range)
{
    return out << distree::Interval(range.start, range.end);
}

bool operator<(const SymbolRange& x, const SymbolRange& y)
{
    return x.end < y.start;
}
}
