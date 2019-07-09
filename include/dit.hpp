#pragma once
#include <memory>
#include <stdexcept>
#include <tuple>

struct DisjointIntervalTree {
    int first, second;
    std::shared_ptr<DisjointIntervalTree> left, right;

    DisjointIntervalTree(int a, int b)
        : left(nullptr)
        , right(nullptr)
    {
        if (a > b) {
            throw std::invalid_argument("DisjointIntervalTree");
        }
        first = a;
        second = b;
    }

    void insert(int start, int end)
    {
        if (start > end) {
            return;
        }
        // case 1: this < other
        if (second < start) {
            return create_or_insert(right, start, end);
        }
        // case 2: this > other
        if (first > end) {
            return create_or_insert(left, start, end);
        }
        // case 3: general case
        auto [a, b, c, d] = boundary(start, end);
        first = b;
        second = c;
        create_or_insert(left, a, b - 1);
        create_or_insert(right, c + 1, d);
    }

private:
    std::tuple<int, int, int, int>
    boundary(int c, int d)
    {
        int a = first;
        int b = second;
        if (a <= c) {
            if (b <= c) {
                return {a, b, c, d};
            }
            if (b <= d) {
                return {a, c, b, d};
            }
            return {a, c, d, b};
        }
        if (a <= d) {
            if (b <= d) {
                return {c, a, b, d};
            }
            return {c, a, d, b};
        }
        return {c, d, a, b};
    }

    void create_or_insert(std::shared_ptr<DisjointIntervalTree>& tree, 
            int start, int end)
    {
        if (!tree) {
            tree = std::make_shared<DisjointIntervalTree>(start, end);
        } else {
            tree->insert(start, end);
        }
    }
};
