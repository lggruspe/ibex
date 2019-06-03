#include "cats.h"
#include <boost/icl/split_interval_set.hpp>

void insert(Alphabet& intervals, char start, char end)
{
    // endpoints included
    // boost::icl::interval<char>::type interval(start, end+1);
    auto interval = boost::icl::interval<char>::closed(start, end);
    intervals += interval;
}

int categorize(const Alphabet& intervals, char c)
{
    const auto& it = intervals.find(c);
    if (it != intervals.end()) {
        return it->lower();
    }
    return -1;
}
