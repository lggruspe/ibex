#pragma once
#ifndef __cplusplus
#include <stdbool.h>
#endif

// parses text
// takes shift and reduce callback functions which get passed arg
bool parse(
    char const *text,
    bool (*shift)(void*, char const*, char const*),
    bool (*reduce)(void*, char const*, char const* const*),
    void *arg);
