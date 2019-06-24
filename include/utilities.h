#include <rnd/regex.h>

namespace regex
{

Expr empty();
Expr optional(Expr);
Expr integer();
Expr number();
Expr identifier();
Expr whitespace();
Expr character();
Expr string();

}
