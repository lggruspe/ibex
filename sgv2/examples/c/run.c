#include "output/scanners.h"
#include "output/scanner_utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_longest_match()
{
    struct input_stack is = is_create(stdin);
    struct scan_output s = match_longest(&is, ALL_RECOGNIZERS);
    print_token(stdout, s.token);
    printf("\n%s\n", s.lexeme);
    free(s.lexeme);
    is_destroy(&is);
}

void run_single_match(char *name)
{
    struct input_stack is = is_create(stdin);
    pardir_output_scanner_utils_h();
    is_destroy(&is);
}

void run_tokenizer()
{
    struct input_stack is = is_create(stdin);
    for (;;) {
        struct scan_output s = match_longest(&is, ALL_RECOGNIZERS);
        if (s.token == TOKEN_EMPTY) {
            free(s.lexeme);
            break;
        }
        print_token(stdout, s.token);
        printf(" %s\n", s.lexeme);
        free(s.lexeme);
    }
    is_destroy(&is);
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
