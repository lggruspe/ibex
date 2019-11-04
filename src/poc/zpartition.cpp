#include "zpartition.hpp"
#include <cassert>
#include <iterator>
#include <limits>
#include <set>
#include <tuple>
#include <utility>

namespace poc
{

const unsigned int lower_bound = 0;
const unsigned int upper_bound = std::numeric_limits<unsigned int>::max();

ZPartitionIterator::ZPartitionIterator(
    std::set<unsigned int>::const_iterator start,
    std::set<unsigned int>::const_iterator end)
    : it(start)
    , end(end)
{}

ZPartitionIterator::ZPartitionIterator(const std::set<unsigned int>& p) 
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

const std::tuple<const unsigned int&, const unsigned int&> ZPartitionIterator::operator*() const
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

ZPartition::ZPartition() : p_(std::set<unsigned int>{lower_bound})
    // contains closed end partition points of [lower_bound, upper_bound)
{}

void ZPartition::insert(unsigned int a, unsigned int b)
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
    std::set<unsigned int> p = p_;
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

std::pair<ZPartitionIterator, ZPartitionIterator>
ZPartition::overlap_range(unsigned int a, unsigned int b) const
{
    auto end = p_.end();
    auto lb_ = p_.lower_bound(a);
    auto ub_ = p_.upper_bound(b);
    ZPartitionIterator lb(lb_, end);
    ZPartitionIterator ub(ub_, end);
    return {lb, ub};
}

}
