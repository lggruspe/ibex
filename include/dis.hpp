#pragma once
#include "dis.h"

class DisjointIntervalSet {
    dis_set set;

public:
    DisjointIntervalSet()
    {
        set = dis_create();
    }

    ~DisjointIntervalSet()
    {
        dis_destroy(&set);
    }

    void insert(int start, int end)
    {
        dis_insert(&set, start, end);
    }
};
