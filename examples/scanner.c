#include "scanner.h"
#include <assert.h>
#include <stdint.h>

typedef uint32_t CodePoint;

// set value of q to next state and return status code (of next state)
// -1: error state (all computations eventually reach a nonaccept state)
// 0: non accept and non error state
// 1: accept state
// TODO: let cases fall-through to equivalent cases (states)
typedef int (*StateTransitionFunction)(int*, CodePoint);

int a(int *q, CodePoint a)
{
    switch (*q) {
    case 0:
        if (a == 97){ *q = 1; return 1; }
        return -1;
    case 1:
        return -1;
    case 2:
        return -1;
    default:
        assert(0);
    }
}

int b(int *q, CodePoint a)
{
    switch (*q) {
    case 0:
        if (a == 98){ *q = 1; return 1; }
        return -1;
    case 1:
        return -1;
    case 2:
        return -1;
    default:
        assert(0);
    }
}

int whitespace(int *q, CodePoint a)
{
    switch (*q) {
    case 0:
        if (a == 9){ *q = 1; return 1; }
        if (a == 10){ *q = 1; return 1; }
        if (a == 32){ *q = 1; return 1; }
        return -1;
    case 1:
        return -1;
    case 2:
        return -1;
    default:
        assert(0);
    }
}

// compute state machine until eof or failure
// return number of transitions to most recent accept state
int compute(StateTransitionFunction f, char const *s)
{
    int checkpoint = 0;
    int q = 0;
    int len = 0;
    int status;
    for (char const *c = s; *c && (status = f(&q, *c)) != -1; ++c) {
        ++len;
        if (status == 1) {
            checkpoint = len;
        }
    }
    return checkpoint;
}

char const *match_longest(char const *s, int *plen)
{
    static StateTransitionFunction recs[] = {
        a,
        b,
        whitespace,
    };
    static char const* names[] = {
        "a",
        "b",
        "whitespace",
    };
    int index = -1;
    int maxlen = 0;
    for (int i = 0; i < 3; ++i) {
        int len = compute(recs[i], s);
        if (len > maxlen) {
            index = i;
            maxlen = len;
        }
    }
    if (plen) {
        *plen = maxlen;
    }
    return index < 0 ? "" : names[index];
}
