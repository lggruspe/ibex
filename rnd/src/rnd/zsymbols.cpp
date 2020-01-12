#include "rnd/zsymbols.hpp"
#include <cassert>
#include <cstdint>
#include <iterator>
#include <limits>
#include <set>
#include <tuple>

namespace rnd {

ZRange::ZRange() {}

ZRange::ZRange(uint32_t start)
    : start(start)
    , end(start == std::numeric_limits<uint32_t>::max() ? start : start+1)
{}

ZRange::ZRange(uint32_t start, uint32_t end) : start(start), end(end)
{
    assert(start <= end);
}

bool ZRange::is_empty() const
{
    return start == end;
}

ZPartition::ZPartition() {}

ZPartition::ZPartition(const std::initializer_list<ZRange>& ranges)
{
    for (const auto& ran: ranges) {
        insert(ran);
    }
}

void ZPartition::insert(const ZRange& ran)
{
    if (!ran.is_empty()) {
        points.insert(ran.start);
        if (ran.end < std::numeric_limits<uint32_t>::max()) {
            points.insert(ran.end);
        }
    }
}

ZPartition ZPartition::combined(const ZPartition& other) const
{
    ZPartition rv = *this;
    rv.points.insert(other.points.begin(), other.points.end());
    return rv;
}

template <class T = std::set<ZRange>::iterator>
std::set<ZRange> ZPartition::to_set(T lb, T ub) const
{
    std::set<ZRange> ranges;
    for (auto it = lb; it != ub; ++it) {
        if (*it != std::numeric_limits<uint32_t>::max()) {
            ranges.insert(ZRange(*it, *std::next(it)));
        }
    }
    return ranges;
}

std::set<ZRange> ZPartition::to_set() const
{
    return to_set(points.begin(), std::prev(points.end()));
}

std::set<ZRange> ZPartition::cover(const ZRange& ran) const
{
    if (ran.is_empty()) {
        return to_set(points.end(), points.end());
    }
    auto lb = points.lower_bound(ran.start);
    if (lb != points.begin() && *lb != ran.start) {
        --lb;
    }
    auto ub = points.upper_bound(ran.end-1);
    return to_set(lb, ub);
}

bool ZRange::operator<(const ZRange& other) const
{
    assert(end <= other.start || other.end <= start || 
        std::tie(start, end) == std::tie(other.start, other.end));
    return std::tie(start, end) < std::tie(other.start, other.end);
}

}
