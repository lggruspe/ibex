#include "swlist.h"

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
