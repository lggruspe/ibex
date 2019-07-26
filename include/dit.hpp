#pragma once
#include <algorithm>
#include <memory>
#include <stdexcept>

struct DisjointIntervalTree {
    int first, second;
    std::shared_ptr<DisjointIntervalTree> left, right;

    DisjointIntervalTree(int first, int second)
        : first(first)
        , second(second)
        , left(nullptr)
        , right(nullptr)
    {
        if (first > second) {
            throw std::invalid_argument("DisjointIntervalTree");
        }
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
        int endpoints[] = { first, second, start, end };
        std::sort(endpoints, endpoints + 4);
        first = endpoints[1];
        second = endpoints[2];
        create_or_insert(left, endpoints[0], endpoints[1] - 1);
        create_or_insert(right, endpoints[2] + 1, endpoints[3]);
    }

private:
    void create_or_insert(std::shared_ptr<DisjointIntervalTree>& tree, 
            int start, int end)
    {
        if (start > end) {
            return;
        }
        if (!tree) {
            tree = std::make_shared<DisjointIntervalTree>(start, end);
        } else {
            tree->insert(start, end);
        }
    }
};
