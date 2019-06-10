#include "parsetree.h"
#include <iostream>

// using Symbol = std::variant<Token, Variable>;
// using Sentence = std::vector<Symbol>;
// using Rule = std::pair<Variable, Sentence>;

template <class Token, class Variable>
void _ParseTree::make_node(const Rule& rule, 
        const std::vector<std::shared_ptr<_ParseTree>>& children)
{

}
