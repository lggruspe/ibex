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
{% for recognizer in scanner %}
{% include "cscanner/_recognizer.c" %}
{% endfor %}
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
        {%- for recognizer in scanner %}
        {{ recognizer.token }},
        {%- endfor %}
    };
    static char const* names[] = {
        {%- for recognizer in scanner %}
        "{{ recognizer.token }}",
        {%- endfor %}
    };
    int index = -1;
    int maxlen = 0;
    for (int i = 0; i < {{ scanner|length }}; ++i) {
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
