#include "distree.h"
#include "test_runner.hpp"
#include "utils.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace distree;

define_test_runner(DistreeTest, DisSet set)

void setup(DistreeTest* test)
{
    test->set.clear();
}

void teardown(DistreeTest* test)
{
    test->set.clear();
}

void test_duplicates(DistreeTest* test)
{
    test->set.insert(0, 1);
    test->set.insert(0, 1);

    test->assert(weight(test->set) == 1);
    test->assert(height(test->set) == 1);

    auto interval = test->set.tree->data;
    test->assert(interval.start == 0 && interval.end == 1);
}

void test_height(DistreeTest* test)
{
    for (int i = 0; i < 100; ++i) {
        test->set.insert(i, i);
    }

    auto n = weight(test->set);
    test->assert(n == 100);
    test->assert((double)(height(test->set)) <= 2*log2(n + 1.0));
}

void test_combine(DistreeTest* test)
{
    DisSet set;
    set.insert(2, 4);
    set.insert(0, 0);
    test->set.insert(1, 3);
    test->set.combine(set);
    test->assert(weight(test->set) == 4);
}

void test_print(DistreeTest* test)
{
    srand(time(nullptr));
    test->assert(true);
    for (int i = 0; i < 100; ++i) {
        auto end = rand() % 100;
        auto start = rand() % (end + 1);
        test->set.insert(start, end);
    }
    std::cout << "test_print: " << test->set << std::endl;
}

int main()
{
    DistreeTest runner(setup, teardown);
    run_test(runner, test_duplicates);
    run_test(runner, test_height);
    run_test(runner, test_combine);
    run_test(runner, test_print);
}
