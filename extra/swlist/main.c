#include "swlist.h"
#include <stdio.h>

sw_register(int)

void sw_print(struct sw_list_int *list) 
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

void pass(int *ignore) { ignore++; }

int main()
{
    struct sw_list_int *tempo = NULL;
    for (int i = 0; i < 10; ++i) {
        tempo = sw_push(tempo, sw_create(i));
    }
    struct sw_list_int *perma = tempo;

    struct sw_list_int *node = perma;
    while (node) {
        sw_init(node, pass);
        node = node->right;
    }

    sw_print(perma);
    tempo = sw_sleep(perma);
    sw_print(tempo);
    sw_wake(perma);
    sw_print(tempo);
    sw_destroy(perma);
}
