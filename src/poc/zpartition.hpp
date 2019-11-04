#pragma once
#include <set>
#include <tuple>
#include <utility>

namespace poc
{

class ZPartitionIterator {
    std::set<unsigned int>::const_iterator it;
    std::set<unsigned int>::const_iterator end;

public:
    ZPartitionIterator(
        std::set<unsigned int>::const_iterator start,
        std::set<unsigned int>::const_iterator end);

    ZPartitionIterator(const std::set<unsigned int>& p);

    bool operator==(const ZPartitionIterator& other) const;

    bool operator!=(const ZPartitionIterator& other) const;

    const std::tuple<const unsigned int&, const unsigned int&> operator*() const;
    
    ZPartitionIterator& operator++();

    ZPartitionIterator operator++(int);
};

class ZPartition {
    std::set<unsigned int> p_;

    ZPartition(std::set<unsigned int> p_) : p_(p_) {}

public:
    ZPartition();

    void insert(unsigned int, unsigned int);

    ZPartition combined(const ZPartition& other) const;

    ZPartitionIterator begin() const;

    ZPartitionIterator end() const;

    std::pair<ZPartitionIterator, ZPartitionIterator>
    overlap_range(unsigned int a, unsigned int b) const;
};

}
