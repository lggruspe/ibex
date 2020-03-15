#include "sagl.h"
#include "grammar-parser/grammar_parser.h"
#include <cstdlib>
#include <cstring>

char const *grammarp_open(char const* bnf)
{
    auto json = parse_grammar(bnf);
    return strdup(json.c_str());
}

void grammarp_close(void* m)
{
    free(m);
}
