#pragma once
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

struct rnd_symbol_interval {
    int start;
    int end;
};

enum rnd_expr_type { 
    RND_SYMBOL = 0,
    RND_UNION = 1,
    RND_CONCATENATION = 2,
    RND_CLOSURE = 3
};

struct rnd_expr {
    enum rnd_expr_type type;
    struct rnd_expr *left;
    struct rnd_expr *right;
    struct rnd_symbol_interval symbols;
};

struct transition {
    int current_state;
    int next_state;
    struct rnd_symbol_interval symbols;
};

struct rnd_dfa {
    size_t number_states;
    size_t number_transitions;
    size_t number_accept_states;
    int start_state;
    struct transition *transitions;
    int *accept_states;
    const char *error;
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
struct rnd_expr *rnd_expr_symbol(int, int);

#ifdef __cplusplus
extern "C" 
#endif
struct rnd_expr *rnd_expr_union(struct rnd_expr*, struct rnd_expr*);

#ifdef __cplusplus
extern "C" 
#endif
struct rnd_expr *rnd_expr_concatenation(struct rnd_expr*, struct rnd_expr*);

#ifdef __cplusplus
extern "C" 
#endif
struct rnd_expr *rnd_expr_closure(struct rnd_expr*);

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
