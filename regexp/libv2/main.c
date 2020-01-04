#include "regexp.h"
#include <stdlib.h>

int main()
{
    char const* re = regexp_open("0|[1-9][0-9]*");
    regexp_close(re);
}
