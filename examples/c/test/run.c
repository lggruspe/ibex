#include "../output/scanners.h"
#include <stdio.h>

void run_longest_match()
{
    struct scan_output s = match_longest(stdin, ALL_RECOGNIZERS);
    print_token(stdout, s.token);
    printf(" \n%s\n", s.lexeme);
    free(s.lexeme);
}

// TODO run_single_match

void run_tokenizer()
{
    for (;;) {
        struct scan_output s = match_longest(stdin, ALL_RECOGNIZERS);
        if (s.token == TOKEN_EMPTY) {
            free(s.lexeme);
            break;
        }
        print_token(stdout, s.token);
        printf(" %s\n", s.lexeme);
        free(s.lexeme);
    }
}

int main(int argc, char **argv)
{
}
