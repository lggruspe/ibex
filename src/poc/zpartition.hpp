#pragma once
#include <set>
#include <tuple>

namespace poc
{

class ZPartitionIterator {
    std::set<int>::const_iterator it;
    std::set<int>::const_iterator end;

public:
    ZPartitionIterator(
        std::set<int>::const_iterator start,
        std::set<int>::const_iterator end);

    ZPartitionIterator(const std::set<int>& p);

    bool operator==(const ZPartitionIterator& other) const;

    bool operator!=(const ZPartitionIterator& other) const;

    const std::tuple<const int&, const int&> operator*() const;
    
    ZPartitionIterator& operator++();

    ZPartitionIterator operator++(int);
};

class ZPartition {
    std::set<int> p_;

    ZPartition(std::set<int> p_) : p_(p_) {}

public:
    ZPartition();

    void insert(int, int);

    ZPartition combined(const ZPartition& other) const;

    ZPartitionIterator begin() const;

    ZPartitionIterator end() const;
};

}
