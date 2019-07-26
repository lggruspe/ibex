#include "tree.hpp"
#include "test_runner.hpp"
#include <iostream>
#include <memory>

define_test_runner(TestRunner,
        rb::Tree<int>* tree,
        tree(nullptr),)

void setup(TestRunner* test)
{
    test->tree = nullptr;
}

void teardown(TestRunner* test)
{
    rb::destroy(test->tree);
}

void test_rb_null(TestRunner* test)
{
    test->assert(!rb::search(test->tree, 0));
}

void test_rb_insert(TestRunner* test)
{
    for (int i = 0; i < 10; ++i) {
        auto node = new rb::Tree<int>(i, rb::Color::red);
        auto [root, replaced] = rb::insert(test->tree, node);
        test->tree = root;
        delete replaced;
    }

    test->assert(test->tree != nullptr);
    for (int i = 0; i < 10; ++i) {
        auto node = rb::search(test->tree, i);
        test->assert(node->data == i);
    }
}

// TODO test duplicates (map)

void test_rb_balanced(TestRunner* test)
{
}

int main()
{
    TestRunner runner(setup, teardown);
    run_test(runner, test_rb_null);
    run_test(runner, test_rb_insert);
    run_test(runner, test_rb_balanced);
}
