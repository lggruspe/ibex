#pragma once
#include <stdio.h>
#include <stdlib.h>
#define data_type int

struct sw_list {
    data_type data;
    struct sw_list *left, *right, *prev, *next;
};

struct sw_list *sw_create(data_type);

struct sw_list *sw_push(struct sw_list*, struct sw_list*);

void sw_destroy(struct sw_list*);

struct sw_list *sw_right(struct sw_list*);

struct sw_list *sw_left(struct sw_list*);

struct sw_list *sw_next(struct sw_list*);

struct sw_list *sw_prev(struct sw_list*);

struct sw_list *sw_sleep(struct sw_list*);

void sw_wake(struct sw_list*);

void sw_init(struct sw_list*);

void sw_print(struct sw_list*);
