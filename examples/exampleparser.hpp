#include "parsetree.hpp"
#include "utils/parser_wrapper.hpp"
#include <memory>
#include <string>
#include <vector>

namespace parser {

struct SyntaxError {
    const char* what() const
    {
        return "syntax error";
    }
};

class ExampleParser;

bool shift_a(ExampleParser*, char const*);
bool shift_b(ExampleParser*, char const*);
bool reduce_S_A(ExampleParser*);
bool reduce_A_aAb(ExampleParser*);
bool reduce_A(ExampleParser*);

class ExampleParser: public BaseParser<ExampleParser> {
    using Parent = BaseParser<ExampleParser>;
public:

    std::vector<std::string> symbols;
    std::vector<std::unique_ptr<ParseTree>> state;

    ExampleParser()
    {
        shift["a"] = shift_a;
        shift["b"] = shift_b;
        reduce["S -> A"] = reduce_S_A;
        reduce["A -> a A b"] = reduce_A_aAb;
        reduce["A ->"] = reduce_A;
    }

    auto ast()
    {
        if (!ok) {
            throw SyntaxError();
        }
        return std::move(state.back());
    }
};

bool shift_symbol(ExampleParser* self, char const* tok, char const* lex)
{
    self->symbols.push_back(tok);
    self->state.push_back(std::make_unique<ParseTree>(lex));
    return true;
}

bool shift_a(ExampleParser* self, char const* lex)
{
    return shift_symbol(self, "a", lex);
}

bool shift_b(ExampleParser* self, char const* lex)
{
    return shift_symbol(self, "b", lex);
}

bool reduce_rule(ExampleParser* self, char const* lhs, const std::vector<std::string>& rhs)
{
    auto node = std::make_unique<ParseTree>(lhs);
    std::vector<decltype(node)> children;
    for (int i = 0; i < (int)rhs.size(); ++i) {
        self->symbols.pop_back();
        children.push_back(std::move(self->state.back()));
        self->state.pop_back();
    }
    while (!children.empty()) {
        node->children.push_back(std::move(children.back()));
        children.pop_back();
    }
    self->symbols.push_back(lhs);
    self->state.push_back(std::move(node));
    return true;
}

bool reduce_S_A(ExampleParser* self)
{
    return reduce_rule(self, "S", {"A"});
}

bool reduce_A_aAb(ExampleParser* self)
{
    return reduce_rule(self, "A", {"a", "A", "b"});
}

bool reduce_A(ExampleParser* self)
{
    return reduce_rule(self, "A", {});
}

}
