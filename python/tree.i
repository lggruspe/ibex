%module tree
%{
#include "../tree.hpp"
%}

%include "../tree.hpp"

%template(Tree) rb::Tree<int>;
%template(search) rb::search<int>;
%template(maximum) rb::maximum<int>;
%template(minimum) rb::minimum<int>;
%template(successor) rb::successor<int>;
%template(predecessor) rb::predecessor<int>;
%template(insert) rb::insert<int>;
