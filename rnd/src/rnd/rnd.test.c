#include "rnd.h"
#include <stdio.h>

int main()
{
    char const* re = rnd_open("0|[1-9][0-9]*");
    printf("%s\n", re);
    rnd_close((void*)re);
}
