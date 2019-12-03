#include "../output/scanners.h"
#include "../output/scanner_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_longest_match()
{
    struct scan_output s = match_longest(stdin, ALL_RECOGNIZERS);
    print_token(stdout, s.token);
    printf(" \n%s\n", s.lexeme);
    free(s.lexeme);
}

void run_single_match(char *name)
{
    pardir_output_scanner_utils_h();
}

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
    if (argc < 2) {
        fprintf(stderr, "usage: run {single,longest,tokenizer} ...\n");
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "single") == 0) {
        if (argc < 4) {
            fprintf(stderr, "usage: run single -s SCANNER\n");
            return EXIT_FAILURE;
        }
        run_single_match(argv[3]);
    } else if (strcmp(argv[1], "longest") == 0) {
        run_longest_match();
    } else if (strcmp(argv[1], "tokenizer") == 0) {
        run_tokenizer();
    }
}
