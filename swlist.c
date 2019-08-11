#include <stdio.h>
#include <stdlib.h>
#define data_type int

struct sw_list {
    data_type data;
    struct sw_list *left, *right, *prev, *next;
};

struct sw_list *sw_create(data_type data)
{
    struct sw_list *node = malloc(sizeof(struct sw_list));
    if (node) {
        node->data = data;
        node->left = node->right = node->prev = node->next = NULL;
    }
    return node;
}

struct sw_list *sw_push(struct sw_list *list, struct sw_list *node)
{
    if (!node) {
        return list;
    }
    node->left = NULL;
    node->right = list;
    if (list) {
        list->left = node;
    }
    return node;
}

void sw_destroy(struct sw_list *list)
{
    while (list) {
        struct sw_list *node = list;
        list = list->right;
        free(node);
    }
}

struct sw_list *sw_right(struct sw_list *node)
{
    return node ? node->right : NULL;
}

struct sw_list *sw_left(struct sw_list *node)
{
    return node ? node->left : NULL;
}

struct sw_list *sw_next(struct sw_list *node)
{
    return node ? node->next : NULL;
}

struct sw_list *sw_prev(struct sw_list *node)
{
    return node ? node->prev : NULL;
}

// return's new head
// if head isn't changed, returns NULL
struct sw_list *sw_sleep(struct sw_list *node)
{
    struct sw_list *prev = sw_prev(node);
    struct sw_list *next = sw_next(node);
    if (next) {
        next->prev = prev;
    }
    if (prev) {
        prev->next = next;
    } else {
        return next;
    }
    return NULL;
}

// not recursive
void sw_wake(struct sw_list *node)
{
    if (node) {
        struct sw_list *prev = sw_prev(node);
        struct sw_list *next = sw_next(node);
        if (prev) {
            prev->next = node;
            node->prev = prev;
        }
        if (next) {
            next->prev = node;
            node->next = next;
        }
    }
}

// should be called on first pass, before waking or sleeping
void sw_init(struct sw_list *node)
{
    if (node) {
        node->prev = sw_left(node);
        node->next = sw_right(node);
    }
}

// for debugging
void sw_print(struct sw_list *list)
{
    while (list && list->prev) {
        list = list->prev;
    }
    printf("(");
    if (list) {
        printf("%d", list->data);
        list = list->next;
    }
    while (list) {
        printf(", %d", list->data);
        list = list->next;
    }
    printf(")\n");
}

int main()
{
    struct sw_list *tempo = NULL;
    for (data_type i = 0; i < 10; ++i) {
        tempo = sw_push(tempo, sw_create(i));
    }
    struct sw_list *perma = tempo;

    struct sw_list *node = perma;
    while (node) {
        sw_init(node);
        node = node->right;
    }

    sw_print(perma);
    tempo = sw_sleep(perma);
    sw_print(tempo);
    sw_wake(perma);
    sw_print(tempo);
    sw_destroy(perma);
}
