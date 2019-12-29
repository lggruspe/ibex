#include "ast.hpp"
#include "scanner.hpp"
#include <sagl>
#include <vector>

enum class Variable { S, A, B };

struct LR1Callback {
    using Symbol = std::variant<Token, Variable>;
    using Rule = std::pair<Symbol, std::vector<Symbol>>;
    std::vector<Symbol> symbols;
    std::vector<ast::Node> state;

    auto accept(bool acc)
    {
        return state.back();
    }

    void shift(Symbol a)
    {
        symbols.push_back(a);
        state.push_back(ast::Node(a));
    }

    void reduce(const Rule& rule)
    {
        ast::Node node(rule.lhs);
        std::vector<ast::Node> children;
        for (int i = 0; i < rule.rhs.size(); ++i) {
            symbols.pop_back();
            auto child = state.back();
            state.pop_back();
        }
        for (const auto& child: children) {
            node.add_child(child);
        }
        state.push_back(node);
    }
};

int main()
{
    sagl::Grammar<Token, Variable> g = {
        {Variable::S, {Variable::A}},
        {Variable::A, {Token::A, Variable::A, Token::B}},
        {Variable::A, {}},
    };
    sagl::Parser p(g);
    p.parse<LR1Callback>(g);
    /*
    sagl::Parser p(R"VOGON(
        S -> A
        A -> a A b
        A ->
        a = a
        b = b 
    )VOGON");
    */
}
