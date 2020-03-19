#include "sagl.h"
#include "grammar-parser/grammar_parser.h"
#include <cstdlib>
#include <cstring>

char const *sagl_open(char const* bnf)
{
    auto json = parse_grammar(bnf);
    return strdup(json.c_str());
}

void sagl_close(void* m)
{
    free(m);
}
