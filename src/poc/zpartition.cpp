#include "zpartition.hpp"
#include <cassert>
#include <iterator>
#include <limits>
#include <set>
#include <tuple>

namespace poc
{

const int lower_bound = 0;
const int upper_bound = std::numeric_limits<int>::max();

ZPartitionIterator::ZPartitionIterator(
    std::set<int>::const_iterator start,
    std::set<int>::const_iterator end)
    : it(start)
    , end(end)
{}

ZPartitionIterator::ZPartitionIterator(const std::set<int>& p) 
    : ZPartitionIterator(p.begin(), p.end())
{}

bool ZPartitionIterator::operator==(const ZPartitionIterator& other) const
{
    return it == other.it;
}

bool ZPartitionIterator::operator!=(const ZPartitionIterator& other) const
{
    return it != other.it;
}

const std::tuple<const int&, const int&> ZPartitionIterator::operator*() const
{
    auto jt = std::next(it);
    if (jt == end) {
        return std::tie(*it, upper_bound);
    }
    return std::tie(*it, *jt);
}

ZPartitionIterator& ZPartitionIterator::operator++()
{
    ++it;
    return *this;
}

ZPartitionIterator ZPartitionIterator::operator++(int)
{
    ZPartitionIterator it(this->it, end);
    operator++();
    return it;
}

ZPartition::ZPartition() : p_(std::set<int>{lower_bound})
    // contains closed end partition points of [lower_bound, upper_bound)
{}

void ZPartition::insert(int a, int b)
{
    // insert nonempty [a, b), must be within [lb, ub)
    assert(a < b && a >= lower_bound && b < upper_bound);
    p_.insert(a);
    p_.insert(b);
}

ZPartition ZPartition::combined(const ZPartition& other) const
{
    // combine nonempty partitions of [lower_bound, upper_bound)
    assert(!p_.empty() && !other.p_.empty());
    std::set<int> p = p_;
    p.insert(other.p_.begin(), other.p_.end());
    return ZPartition(p);
}

ZPartitionIterator ZPartition::begin() const
{
    return ZPartitionIterator(p_);
}

ZPartitionIterator ZPartition::end() const
{
    return ZPartitionIterator(p_.end(), p_.end());
}

}
