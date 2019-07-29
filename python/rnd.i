%module rnd
%{
#include "../include/rnd/tree.hpp"
#include "../include/rnd/distree.h"
#include "../include/rnd/alphabet.hpp"
#include "../include/rnd/regex.h"
#include "../include/rnd/nfa.h"
#include "../include/rnd/dfa.h"

%}

%ignore distree::operator<<;
%ignore regex::operator<<;
%ignore automata::operator<<;

%include "../include/rnd/tree.hpp"
%include "../include/rnd/distree.h"
%include "../include/rnd/alphabet.hpp"
%include "../include/rnd/regex.h"
%include "../include/rnd/nfa.h"
%include "../include/rnd/dfa.h"
