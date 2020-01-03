#include "regexp.h"

int main()
{
    struct rnd_dfa m = rnd_parse_regex("0|[1-9][0-9]*");
    rnd_dfa_destroy(&m);
}
