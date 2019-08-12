#include "swlist.h"

sw_register(int)

int main()
{
    struct sw_list_int *tempo = NULL;
    for (int i = 0; i < 10; ++i) {
        tempo = sw_push(tempo, sw_create(i));
    }
    struct sw_list_int *perma = tempo;

    struct sw_list_int *node = perma;
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
