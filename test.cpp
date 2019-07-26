#include "tree.hpp"
#include "test_runner.hpp"
#include <cmath>

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
        test->assert(node && node->data == i);
    }
}

template <class T>
size_t height(rb::Tree<T>* tree)
{
    if (!tree) {
        return 0;
    }
    auto left = height(tree->left);
    auto right = height(tree->right);
    return 1 + (left > right ? left : right);
}

template <class T>
size_t weight(rb::Tree<T>* tree)
{
    if (!tree) {
        return 0;
    }
    auto left = weight(tree->left);
    auto right = weight(tree->right);
    return 1 + left + right;
}

void test_rb_balanced(TestRunner* test)
{
    for (int i = 0; i < 100; ++i) {
        auto [root, replaced] = rb::insert(test->tree,
                new rb::Tree<int>(i, rb::Color::red));
        test->tree = root;
        delete replaced;
    }

    auto h = height(test->tree);
    auto n = weight(test->tree);
    test->assert(n == 100);
    test->assert(h <= 2*log2(n+1));
}

void test_rb_duplicates(TestRunner* test)
{
    for (int i = 0; i < 10; ++i) {
        auto [root, replaced] = rb::insert(test->tree,
                new rb::Tree<int>(0, rb::Color::red));
        test->tree = root;
        delete replaced;
    }
    test->assert(weight(test->tree) == 1);
}

int main()
{
    TestRunner runner(setup, teardown);
    run_test(runner, test_rb_null);
    run_test(runner, test_rb_insert);
    run_test(runner, test_rb_balanced);
    run_test(runner, test_rb_duplicates);
}
