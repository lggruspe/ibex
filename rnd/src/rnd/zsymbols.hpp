#pragma once
#include <cassert>
#include <cstdint>
#include <iterator>
#include <limits>
#include <set>
#include <tuple>

namespace rnd {

// Represents a half-open interval [start, end) with integer endpoints.
struct ZRange {
    uint32_t start = std::numeric_limits<uint32_t>::max();
    uint32_t end = std::numeric_limits<uint32_t>::max();

    ZRange() {}

    ZRange(uint32_t start)
        : start(start)
        , end(start == std::numeric_limits<uint32_t>::max() ? start : start+1)
    {}

    ZRange(uint32_t start, uint32_t end) : start(start), end(end)
    {
        assert(start <= end);
    }

    bool is_empty() const
    {
        return start == end;
    }

    // Comparator for storing ZRanges in a set/map.
    // Asserts that the two ZRanges are either disjoint or are equal.
    bool operator<(const ZRange& other) const
    {
        assert(end <= other.start || other.end <= start || 
            std::tie(start, end) == std::tie(other.start, other.end));
        return std::tie(start, end) < std::tie(other.start, other.end);
    }
};

// Represents a partition of the set of integers containing half-open intervals.
struct ZPartition {
    std::set<uint32_t> points = {0, std::numeric_limits<uint32_t>::max()};

    ZPartition() {}

    ZPartition(const std::initializer_list<ZRange>& ranges)
    {
        for (const auto& ran: ranges) {
            insert(ran);
        }
    }

    void insert(const ZRange& ran)
    {
        if (!ran.is_empty()) {
            points.insert(ran.start);
            if (ran.end < std::numeric_limits<uint32_t>::max()) {
                points.insert(ran.end);
            }
        }
    }

    ZPartition combined(const ZPartition& other) const
    {
        ZPartition rv = *this;
        rv.points.insert(other.points.begin(), other.points.end());
        return rv;
    }

    template <class T = std::set<ZRange>::iterator>
    std::set<ZRange> to_set(T lb, T ub) const
    {
        std::set<ZRange> ranges;
        for (auto it = lb; it != ub; ++it) {
            if (*it != std::numeric_limits<uint32_t>::max()) {
                ranges.insert(ZRange(*it, *std::next(it)));
            }
        }
        return ranges;
    }

    template <class T = std::set<ZRange>::iterator>
    std::set<ZRange> to_set() const
    {
        return to_set(points.begin(), std::prev(points.end()));
    }

    auto cover(const ZRange& ran) const
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
};

}
