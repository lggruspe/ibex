#include "scanner.c"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *mygetline()
{
    char *line = NULL;
    size_t n;
    getline(&line, &n, stdin);
    return line;
}

void run_longest_match()
{
    int n;
    char *s = mygetline();
    char const *tok = match_longest(s, &n);
    char *lex = strndup(s, n);
    printf("%s\n%s\n", tok, lex);
    free(lex);
    free(s);
}

void run_single_match(char *name)
{
    char *s = mygetline();
    pardir_output_scanner_utils_h();
    free(s);
}

void tokenize(char *s)
{
    int n;
    for (;;) {
        char const *tok = match_longest(s, &n);
        if (n == 0) {
            break;
        }
        char *lex = strndup(s, n);
        printf("%s %s\n", tok, lex);
        free(lex);
        s += n;
    }
}

void run_tokenizer()
{
    for (;;) {
        char *s = mygetline();
        tokenize(s);
        if (strlen(s) == 0) {
            free(s);
            break;
        }
        free(s);
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
