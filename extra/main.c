#include "rnd.h"

int main()
{
    struct rnd_expr *expr = rnd_expr_union(
            rnd_expr_union(
                rnd_expr_symbol(0, 1),
                rnd_expr_symbol(1, 2)
                ),
            rnd_expr_symbol(2, 3)
            );

    struct rnd_dfa dfa = rnd_convert(expr);
    rnd_expr_destroy(expr);
    rnd_dfa_destroy(&dfa);
}
