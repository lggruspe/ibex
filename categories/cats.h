#pragma once
#include <boost/icl/split_interval_set.hpp>
#include <iostream>

class Alphabet {
public:
    // represented as a set of non-overlapping intervals
    boost::icl::split_interval_set<char> intervals;

    void insert(char start, char end)
    {
        // closed range 
        //boost::icl::interval<char>::type interval(start, end+1);
        auto interval = boost::icl::interval<char>::closed(start, end);
        intervals += interval;
    }

    int categorize(char c) const 
    {
        const auto& it = intervals.find(c);
        if (it != intervals.end()) {
            return it->lower();
        }
        return -1;      // not found
    }

    Alphabet operator+(const Alphabet& other) const
    {
        Alphabet res;
        res.intervals = intervals + other.intervals;
        return res;
    }
};

std::ostream& operator<<(std::ostream& out, const Alphabet& alphabet)
{
    return (out << alphabet.intervals);
}
