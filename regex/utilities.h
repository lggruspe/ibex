#ifndef REGEX_UTILITIES_H
#define REGEX_UTILITIES_H

#include "regex.h"

extern Re empty;
extern Re zero;
extern Re nonzero;
extern Re digit;
extern Re positive;
extern Re integer;
extern Re real;
extern Re lower;
extern Re upper;
extern Re letter;
extern Re identifier;

void re_load_defaults();

#endif
