#pragma once
#include <rnd.h>

#ifdef __cplusplus
extern "C"
#endif
struct rnd_dfa rnd_parse_regex(char const *re);
