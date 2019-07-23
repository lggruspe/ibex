#include "dis.h"
#include "dis.hpp"
#include "dit.hpp"
#include "dis2.h"
#include <boost/icl/split_interval_set.hpp>
#include <cstdlib>
#include <ctime>

size_t n = 2000;
int start[2000];
int end[2000];

void init_intervals()
{
    for (size_t i = 0; i < n; ++i) {
        end[i] = rand() % (int)n;
        start[i] = rand() % (end[i] + 1);
    }
}

void run_dis()
{
    struct dis_set intervals = dis_create();
    for (size_t i = 0; i < n; ++i) {
        dis_insert(&intervals, start[i], end[i]);
    }
    dis_destroy(&intervals);
}

void run_dis2()
{
    struct dis2_set intervals = dis2_create();
    for (size_t i = 0; i < n; ++i) {
        dis2_insert(&intervals, start[i], end[i]);
    }
    dis2_destroy(&intervals);
}

void run_disxx()
{
    DisjointIntervalSet intervals;
    for (size_t i = 0; i < n; ++i) {
        intervals.insert(start[i], end[i]);
    }
}

void run_dit()
{
    DisjointIntervalTree intervals(start[0], end[0]);
    for (size_t i = 1; i < n; ++i) {
        intervals.insert(start[i], end[i]);
    }
}

void run_icl()
{
    boost::icl::split_interval_set<int> intervals;
    for (size_t i = 0; i < n; ++i) {
        intervals += boost::icl::interval<int>::type(start[i], end[i]);
    }
}

int main()
{
    init_intervals();
    run_dis();
    run_dis2();
    run_disxx();
    run_dit();
    run_icl();
}
