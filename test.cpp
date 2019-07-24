#include "tree.hpp"
#include <iostream>
#include <memory>

#define run_test(test) do {\
    std::cout << #test << " " << (test() ? "passed" : "failed") << std::endl;\
} while (0)

struct TestState {
    size_t passed;
    size_t total;

    TestState() : passed(0), total(0) {}

    void assert(bool condition)
    {
        ++total;
        if (condition) {
            ++passed;
        }
    }

    bool passes() const
    {
        return passed > 0 && total == passed;
    }
};


bool test_rb_null()
{
    TestState state;
    std::shared_ptr<rb::Tree<int>> tree = nullptr;
    state.assert(!rb::search(tree, 0));
    return state.passes();
}

bool test_rb_insert()
{
    TestState state;
    std::shared_ptr<rb::Tree<int>> tree = nullptr;
    for (int i = 0; i < 10; ++i) {
        auto node = std::make_shared<rb::Tree<int>>(i, rb::Color::red);
        auto [root, _] = rb::insert(tree, node);
        tree = root;
    }
    state.assert(tree != nullptr);
    for (int i = 0; i < 10; ++i) {
        auto node = rb::search(tree, i);
        state.assert(node->data == i);
    }
    return state.passes();
}

// TODO test duplicates (map)

bool test_rb_balanced()
{
    return false;
}

int main()
{
    run_test(test_rb_null);
    run_test(test_rb_insert);
    run_test(test_rb_balanced);
}
