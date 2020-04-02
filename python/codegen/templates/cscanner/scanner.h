#pragma once

// returns token and sets value of plen to length of lexeme if plen != NULL
// "" if no match
char const *match_longest(char const *s, int *plen);
