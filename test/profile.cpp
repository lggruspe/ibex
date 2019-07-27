#include "distree.hpp"
#include <boost/icl/split_interval_set.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

size_t n = 10000;
int start[10000];
int end[10000];

void init_intervals()
{
    /*
    for (size_t i = 0; i < n/2; ++i) {
        start[i] = n/2 - i;
        end[i] = n/2 + i;
    }
    */

    for (int i = 0; i < (int)n; ++i) {
        end[i] = rand();
        start[i] = rand() % (end[i] + 1);
    }
}

void run_icl()
{
    boost::icl::split_interval_set<int> intervals;
    for (size_t i = 0; i < n; ++i) {
        intervals += boost::icl::interval<int>::type(start[i], end[i]);
    }
}

void run_distree()
{
    distree::DisSet set;
    for (size_t i = 0; i < n; ++i) {
        set.insert(start[i], end[i]);
    }
}

int main()
{
    srand(time(NULL));
    init_intervals();
    run_icl();
    run_distree();
}
