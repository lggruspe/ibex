#include "alphabet.hpp"

namespace rnd
{

bool is_empty(const SymbolRange& range)
{
    return range.start == 0 && range.end == 0;
}

bool overlaps(const distree::Interval& t, const distree::Interval& u)
{
    return u == t;
}

}
