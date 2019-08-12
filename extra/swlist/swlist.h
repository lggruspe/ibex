#pragma once
#include <stdlib.h>

#define sw_register(data_type) \
 \
struct sw_list_##data_type { \
    data_type data; \
    struct sw_list_##data_type *left, *right, *prev, *next; \
}; \
 \
struct sw_list_##data_type *sw_create(data_type data) \
{ \
    struct sw_list_##data_type *node = malloc(sizeof(struct sw_list_##data_type)); \
    if (node) { \
        node->data = data; \
        node->left = node->right = node->prev = node->next = NULL; \
    } \
    return node; \
} \
 \
struct sw_list_##data_type *sw_push(struct sw_list_##data_type *list, struct sw_list_##data_type *node) \
{ \
    if (!node) { \
        return list; \
    } \
    node->left = NULL; \
    node->right = list; \
    if (list) { \
        list->left = node; \
    } \
    return node; \
} \
 \
void sw_destroy(struct sw_list_##data_type *list) \
{ \
    while (list) { \
        struct sw_list_##data_type *node = list; \
        list = list->right; \
        free(node); \
    } \
} \
 \
/* return's new head
if head isn't changed, returns NULL */\
struct sw_list_##data_type *sw_sleep(struct sw_list_##data_type *node) \
{ \
    struct sw_list_##data_type *prev = sw_prev(node); \
    struct sw_list_##data_type *next = sw_next(node); \
    if (next) { \
        next->prev = prev; \
    } \
    if (prev) { \
        prev->next = next; \
    } else { \
        return next; \
    } \
    return NULL; \
} \
 \
/* not recursive */\
void sw_wake(struct sw_list_##data_type *node) \
{ \
    if (node) { \
        struct sw_list_##data_type *prev = sw_prev(node); \
        struct sw_list_##data_type *next = sw_next(node); \
        if (prev) { \
            prev->next = node; \
            node->prev = prev; \
        } \
        if (next) { \
            next->prev = node; \
            node->next = next; \
        } \
    } \
} \
 \
/* should be called on first pass, before waking or sleeping */\
void sw_init(struct sw_list_##data_type *node) \
{ \
    if (node) { \
        node->prev = sw_left(node); \
        node->next = sw_right(node); \
    } \
}

#define sw_right(node) ((node) ? (node)->right : NULL)

#define sw_left(node) ((node) ? (node)->left : NULL)

#define sw_next(node) ((node) ? (node)->next : NULL)

#define sw_prev(node) ((node) ? (node)->prev : NULL)
