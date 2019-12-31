#pragma once
#include "../../rnd/src/zsymbols.hpp"
#include "../scan/scanner.hpp"
#include "variable.hpp"
#include <cstdint>
#include <exception>
#include <iterator>
#include <limits>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace ast {

struct InvalidReduce : public std::logic_error {
    InvalidReduce() : std::logic_error("invalid reduce") {}
};

template <class Symbol>
struct Node {
    virtual ~Node() = 0;
};

template <class Symbol>
Node<Symbol>::~Node()
{}

template <class Symbol>
struct TemporaryNode : public Node<Symbol> {
    Symbol token;

    TemporaryNode(const std::pair<Symbol, std::string>& lookahead)
        : token(lookahead.first)
    {
        if (token != Symbol(scanner::Token::PIPE)
                && token != Symbol(scanner::Token::STAR)
                && token != Symbol(scanner::Token::LPAREN)
                && token != Symbol(scanner::Token::RPAREN)
                && token != Symbol(scanner::Token::QUESTION)
                && token != Symbol(scanner::Token::PLUS)) {
            throw std::logic_error("incorrect use of TemporaryNode");
        }
    }
};

template <class Symbol>
struct OperatorNode : public Node<Symbol> {
    using Pointer = std::unique_ptr<Node<Symbol>>;
    enum class Type { Union, Concatenation, Closure };
    Type type;
    Pointer left = nullptr;
    Pointer right = nullptr;

    OperatorNode(Type type, Pointer& left, Pointer& right = nullptr)
        : type(type)
        , left(std::move(left))
        , right(std::move(right))
    {}
};

template <class Symbol>
struct ValueNode : public Node<Symbol> {
    ZRange value;

    ValueNode() {}

    ValueNode(const std::pair<Symbol, std::string>& lookahead)
    {
        if (auto token = lookahead.first; token == Symbol(scanner::Token::DOT)) {
            value = ZRange(0, std::numeric_limits<uint32_t>::max());
        } else if (token == Symbol(scanner::Token::INTERVAL)) {
            if (lookahead.second.size() != 5) {
                throw std::logic_error("TODO");
            }
            uint32_t a = lookahead.second[1];
            uint32_t b = lookahead.second[3];
            if (b == std::numeric_limits<uint32_t>::max()) {
                --b;
            }
            ++b;
            value = ZRange(a, b);
        } else if (token == Symbol(scanner::Token::SYMBOL)) {
            if (lookahead.second.size() != 1) {
                throw std::logic_error("TODO");
            }
            uint32_t a = lookahead.second[1];
            value = ZRange(a, a+1);
        } else {
            throw std::logic_error("incorrect use of ValueNode");
        }
    }
};

template <class Symbol>
class Callback {
    using OperatorType = typename OperatorNode<Symbol>::Type;
public:

    using Pointer = std::unique_ptr<Node<Symbol>>;
    using Rule = std::pair<Symbol, std::vector<Symbol>>;
    std::vector<Pointer> stack;

    auto accept(bool acc)
    {
        if (!acc) {
            throw std::logic_error("syntax error");
        }
        return std::move(stack.back());
    }

    void shift(const std::pair<Symbol, std::string>& lookahead)
    {
        Pointer node = nullptr;
        if (auto token = lookahead.first; token == Symbol(scanner::Token::PIPE)
                || token == Symbol(scanner::Token::STAR)
                || token == Symbol(scanner::Token::LPAREN)
                || token == Symbol(scanner::Token::RPAREN)
                || token == Symbol(scanner::Token::QUESTION)
                || token == Symbol(scanner::Token::PLUS)) {
            node = Pointer(new TemporaryNode(lookahead));
        } else if (token == Symbol(scanner::Token::DOT)
                || token == Symbol(scanner::Token::INTERVAL)
                || token == Symbol(scanner::Token::SYMBOL)) {
            node = Pointer(new ValueNode(lookahead));
        } else {
            throw std::logic_error("invalid shift");
        }
        stack.push_back(std::move(node));
    }

    auto reduce_expr(std::list<Pointer>& rhs) const
    {
        if (auto size = rhs.size(); size == 1) {
            return std::move(rhs.front());
        } else if (size == 3) {
            // TODO check if middle element is a PIPE
            return Pointer(new OperatorNode<Symbol>(
                OperatorType::Union,
                rhs.front(),
                rhs.back()));
        } else {
            throw InvalidReduce();
        }
    }

    auto reduce_term(std::list<Pointer>& rhs) const
    {
        if (auto size = rhs.size(); size == 1) {
            return std::move(rhs.front());
        } else if (size == 2) {
            return Pointer(new OperatorNode<Symbol>(
                OperatorType::Concatenation,
                rhs.front(),
                rhs.back()));
        } else {
            throw InvalidReduce();
        }
    }

    auto reduce_factor(std::list<Pointer>& rhs) const
    {
        if (auto size = rhs.size(); size == 1) {
            return std::move(rhs.front());
        } else if (size == 2) {
            /*
            Pointer node = nullptr;
            switch (rhs.back()->token) {
            case Symbol(scanner::Token::STAR):
                node = Pointer(new OperatorNode<Symbol>(
                    OperatorType::Closure,
                    std::move(rhs.front())));
                break;
            case Symbol(scanner::Token::PLUS): {
                throw std::logic_error("TODO");
                break;
            }
            case Symbol(scanner::Token::QUESTION): {
                node = Pointer(new OperatorNode<Symbol>(
                    OperatorType::Union,
                    rhs.front(),
                    Pointer(new ValueNode<Symbol>()))
                break;
            }
            default:
                throw InvalidReduce();
            }
            return node;
            */
            throw std::logic_error("TODO");
        } else {
            throw InvalidReduce();
        }
    }

    auto reduce_value(std::list<Pointer>& rhs) const
    {
        if (auto size = rhs.size(); size == 1) {
            return std::move(rhs.front());
        } else if (size == 3) {
            return std::move(*std::next(rhs.begin()));
        } else {
            throw InvalidReduce();
        }
    }

    void reduce(const Rule& rule)
    {
        std::list<Pointer> rhs;
        for (int i = 0; i < (int)(rule.second.size()); ++i) {
            rhs.push_front(std::move(stack.back()));
            stack.pop_back();
        }
        Pointer node = nullptr;
        if (auto lhs = rule.first; lhs == Symbol(Variable::EXPR)) {
            node = std::move(reduce_expr(rhs));
        } else if (lhs == Symbol(Variable::TERM)) {
            node = std::move(reduce_term(rhs));
        } else if (lhs == Symbol(Variable::FACTOR)) {
            node = std::move(reduce_factor(rhs));
        } else if (lhs == Symbol(Variable::VALUE)) {
            node = std::move(reduce_value(rhs));
        } else {
            throw InvalidReduce();
        }
        stack.push_back(std::move(node));
    }
};

}