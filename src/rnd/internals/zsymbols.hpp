#pragma once
#include <cstdint>
#include <limits>
#include <set>

namespace rnd {

// Represents a half-open interval [start, end) with integer endpoints.
struct ZRange {
    uint32_t start = std::numeric_limits<uint32_t>::max();
    uint32_t end = std::numeric_limits<uint32_t>::max();

    ZRange();

    ZRange(uint32_t start);

    ZRange(uint32_t start, uint32_t end);

    bool is_empty() const;

    // Comparator for strong ZRanges in a set/map.
    // Asserts that two ZRanges in a set/map are either disjoint or equal.
    bool operator<(const ZRange&) const;
};


// Represents a partition of the set of integers containing half-open intervals.
struct ZPartition {
    std::set<uint32_t> points = {0, std::numeric_limits<uint32_t>::max()};

    ZPartition();

    ZPartition(const std::initializer_list<ZRange>& ranges);

    void insert(const ZRange& ran);

    ZPartition combined(const ZPartition& other) const;

    template <class T = std::set<ZRange>::iterator>
    std::set<ZRange> to_set(T lb, T ub) const;

    std::set<ZRange> to_set() const;

    std::set<ZRange> cover(const ZRange& ran) const;
};

}
