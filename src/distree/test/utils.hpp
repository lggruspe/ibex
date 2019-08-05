#pragma once
#include "distree.h"
#include <iostream>

void print_set()
{
    size_t n = 30;
    int start[n];
    int end[n];

    distree::DisSet set;
    for (size_t i = 0; i < n; ++i) {
        std::cout << "i = " << i << std::endl;
        std::cout << "set = " << set << std::endl;
        end[i] = rand() % 100;
        start[i] = rand() % (end[i] + 1);
        set.insert(start[i], end[i]);

        std::cout << "(start, end)(i) = (" << start[i] << ", " << end[i] << ")" << std::endl;
    }
    std::cout << "i = " << n << ", set = " << set << std::endl;
}

size_t node_weight(distree::DisTree *node)
{
    if (!node) {
        return 0;
    }
    return 1 + node_weight(node->left) + node_weight(node->right);
}

size_t weight(const distree::DisSet& set)
{
    return node_weight(set.tree);
}

size_t node_height(distree::DisTree *node)
{
    if (!node) {
        return 0;
    }
    size_t l = node_height(node->left);
    size_t r = node_height(node->right);
    return 1 + (l > r ? l : r);
}

size_t height(const distree::DisSet& set)
{
    return node_height(set.tree);
}
