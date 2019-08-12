#include "vector.h"
#include "swlist/swlist.h"
#include <stdio.h>

#define fail_if_empty(input) do {\
    int c = (input);\
    if (c == 0 || c == EOF) return -1;\
} while (0)

enum token {
## for scanner in scanners
    token_{{ scanner.token|upper }},
## endfor
};

struct scanner {
    enum token token;
    int state;
    int start;
    int (*transition)(int, int);
};

## for scanner in scanners
{% include "scanner.c" %}

## endfor

vector_register(int)

typedef struct scanner scanner;
sw_register(scanner)

int main()
{
    struct vector_int *lexeme = NULL;
    struct sw_list_scanner *scanners = NULL;
## for scanner in scanners|reverse
    scanners = sw_push(scanners, sw_create((scanner) {
        .token = token_{{ scanner.token|upper }},
        .state = -1,
        .start = 0,
        .transition = transition_{{ scanner.token }}
    }));
## endfor

    sw_destroy(scanners);
}
