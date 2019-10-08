#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <set>
#include <vector>

struct ClosedInterval {
    mutable int start;
    mutable int end;

    ClosedInterval(int start, int end) : start(start), end(end) {}

    ClosedInterval() : ClosedInterval(0, 0) {}

    // true iff not empty
    // TODO [0, 0] should be considered nonempty, create a separate function
    // for comparing with epsilon
    explicit operator bool() const
    {
        return start < end || (start == end && start != 0);
    }

    // induces a non-transitive == operator (tests for non-disjointness rather
    // than equality
    bool operator<(const ClosedInterval& other) const
    {
        return end < other.start;
    }
};

// creates partition generated by endpoints of a and b, assuming a and b are
// overlapping
std::array<ClosedInterval, 3> disjoint_union(
    const ClosedInterval& a,
    const ClosedInterval& b)
{
    assert(!(a < b) && !(b < a));
    int endpoints[4] = {a.start, a.end, b.start, b.end};
    std::sort(endpoints, endpoints+4);
    return {
        ClosedInterval(endpoints[0], endpoints[1]-1),
        ClosedInterval(endpoints[1], endpoints[2]),
        ClosedInterval(endpoints[2]+1, endpoints[3]),
    };
}

using ClosedIntervalSet = std::set<ClosedInterval>;

void insert_interval(ClosedIntervalSet& A, const ClosedInterval& b)
{
    assert(b);
    std::vector<ClosedInterval> B = {b};
    while (!B.empty()) {
        auto b = B.back();
        B.pop_back();
        auto [it, ok] = A.insert(b);
        assert(*it);        // all elements in A must be nonempty
        if (!ok) {
            auto partition = disjoint_union(*it, b);
            assert(partition[1]);   // true as long as a and b overlap
            it->start = partition[1].start;
            it->end = partition[1].end;
            if (partition[2]) {
                B.push_back(partition[2]);
            }
            if (partition[0]) {
                B.push_back(partition[0]);
            }
        }
    }
}

ClosedIntervalSet combine_interval_sets(
    const ClosedIntervalSet& A,
    const ClosedIntervalSet& B)
{
    ClosedIntervalSet C = A;
    for (const auto& b: B) {
        insert_interval(C, b);
    }
    return C;
}

auto overlap_range(const ClosedIntervalSet& A, const ClosedInterval& a)
{
    // undefined behavior, but works on g++ 9.1.0
    return A.equal_range(a);
}

std::ostream& operator<<(std::ostream& os, const ClosedInterval& a)
{
    if (!a) {
        return os << "nil";
    }
    return os << "[" << a.start << ", " << a.end << "]";
}
