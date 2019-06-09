#include "regex2.h"

namespace regex2
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
