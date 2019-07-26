#include "dis.h"
#include "dis.hpp"
#include "distree.hpp"
#include <boost/icl/split_interval_set.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

size_t n = 3000;
int start[3000];
int end[3000];

void init_intervals()
{
    for (size_t i = 0; i < n/2; ++i) {
        start[i] = n/2 - i;
        end[i] = n/2 + i;
    }
}

template <class T>
size_t node_weight(T* node)
{
    if (!node) {
        return 0;
    }
    size_t left = node_weight(node->left);
    size_t right = node_weight(node->right);
    return 1 + left + right;
}

size_t weight(struct dis_set *tree)
{
    return node_weight(tree->root);
}
size_t weight(distree::DisSet* tree)
{
    return node_weight(tree->tree);
}

template <class T>
size_t node_height(T* node)
{
    if (!node) {
        return 0;
    }
    size_t left = node_height(node->left);
    size_t right = node_height(node->right);
    return 1 + (left > right ? left : right);
}

size_t height(struct dis_set* tree)
{
    return node_height(tree->root);
}
size_t height(distree::DisSet* tree)
{
    return node_height(tree->tree);
}

template <class T>
void print_stats(const std::string& name, T& t)
{
    std::cout << name << std::endl;
    std::cout << "weight: " << weight(&t) << std::endl;
    std::cout << "height: " << height(&t) << std::endl;
}

void run_dis()
{
    struct dis_set intervals = dis_create();
    for (size_t i = 0; i < n; ++i) {
        dis_insert(&intervals, start[i], end[i]);
    }
    print_stats("run_dis", intervals);
    dis_destroy(&intervals);
}

void run_disxx()
{
    DisjointIntervalSet intervals;
    for (size_t i = 0; i < n; ++i) {
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

void run_distree()
{
    distree::DisSet set;
    for (size_t i = 0; i < n; ++i) {
        set.insert(start[i], end[i]);
    }
    print_stats("run_distree", set);
}

int main()
{
    init_intervals();
    run_dis();
    run_disxx();
    run_icl();
    run_distree();
}
