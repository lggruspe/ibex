#include "regexp.h"
#include <stdio.h>

int main()
{
    char const* re = regexp_open("0|[1-9][0-9]*");
    printf("%s\n", re);
    regexp_close(re);
}
