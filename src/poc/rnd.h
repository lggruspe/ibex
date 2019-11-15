#pragma once
#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#endif

struct rnd_range {
    uint32_t start;
    uint32_t end;
};

enum rnd_type { 
    RND_SYMBOL = 0,
    RND_UNION = 1,
    RND_CONCATENATION = 2,
    RND_CLOSURE = 3
};

struct rnd_expr {
    enum rnd_type type;
    struct rnd_range value;
    struct rnd_expr *left;
    struct rnd_expr *right;
};

struct rnd_transition {
    struct rnd_range label;
    int to;
};

struct rnd_state {
    bool accept;
    int outdegree;
    struct rnd_transition *transitions;
};

// order: number of states
// states:
//      an array of states. indices serve as state ids
//      invariant: states[0] is the start state
// error: explicit error state (non-accept state that transitions to itself)
struct rnd_dfa {
    int order;
    struct rnd_state *states;
    int error;
};

#ifdef __cplusplus
extern "C" 
#endif
struct rnd_dfa rnd_convert(struct rnd_expr*);

#ifdef __cplusplus
extern "C" 
#endif
void rnd_dfa_destroy(struct rnd_dfa*);

#ifdef __cplusplus
extern "C" 
#endif
struct rnd_expr *rnd_symbol(uint32_t, uint32_t);

#ifdef __cplusplus
extern "C" 
#endif
struct rnd_expr *rnd_union(struct rnd_expr*, struct rnd_expr*);

#ifdef __cplusplus
extern "C" 
#endif
struct rnd_expr *rnd_concatenation(struct rnd_expr*, struct rnd_expr*);

#ifdef __cplusplus
extern "C" 
#endif
struct rnd_expr *rnd_closure(struct rnd_expr*);

#ifdef __cplusplus
extern "C" 
#endif
void rnd_expr_destroy(struct rnd_expr*);

#ifdef __cplusplus
extern "C"
#endif
void rnd_expr_free(struct rnd_expr*);

#ifdef __cplusplus
extern "C"
#endif
int rnd_get_expr_counter();
