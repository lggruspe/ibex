#pragma once
#include "parser.h"
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace parser {

struct ParseTree {
    using Symbol = std::string;
    using Tree = std::unique_ptr<ParseTree>;
    std::vector<Tree> children;
    Symbol value;

    ParseTree(Symbol value) : value(value) {}
};

std::ostream& operator<<(
    std::ostream& out,
    std::unique_ptr<ParseTree>& node)
{
    if (node == nullptr) {
        return out << "nil";
    }
    if (!node->children.empty()) {
        out << "(";
    }
    out << node->value;
    for (auto& child: node->children) {
        out << " " << child;
    }
    if (!node->children.empty()) {
        out << ")";
    }
    return out;
}

bool shift(void*, char const*, char const*);
bool reduce(void*, char const*, char const* const*);
bool shift_symbol(void* self, char const* tok, char const* lex);
bool reduce_rule(void* self, char const* lhs, char const* const* rhs);

struct SyntaxError {
    const char* what() const
    {
        return "SyntaxError";
    }
};

class ExampleParser {
public:
    std::map<std::string, bool (*)(void*, char const*, char const*)> shift_handlers;
    std::map<std::pair<std::string, std::vector<std::string>>, bool (*)(void*, char const*, char const* const*)> reduce_handlers;

    std::vector<std::string> symbols;
    std::vector<std::unique_ptr<ParseTree>> state;

    auto ast(bool ok)
    {
        if (!ok) {
            throw SyntaxError();
        }
        return std::move(state.back());
    }

    ExampleParser()
    {
        add_shift_handler("a", shift_symbol);
        add_shift_handler("b", shift_symbol);
        add_reduce_handler("S", {"A"}, reduce_rule);
        add_reduce_handler("A", {"a", "A", "b"}, reduce_rule);
        add_reduce_handler("A", {}, reduce_rule);
    }

    bool parse(char const* text)
    {
        return ::parse(text, shift, reduce, (void*)this);
    }

    void add_shift_handler(char const* tok, bool (*handler)(void*, char const*, char const*))
    {
        shift_handlers[tok] = handler;
    }

    void add_reduce_handler(char const* lhs, const std::vector<std::string>& rhs, bool (*handler)(void*, char const*, char const* const*))
    {
        reduce_handlers[std::make_pair(lhs, rhs)] = handler;
    }
};

bool shift_symbol(void* self, char const* tok, char const* lex)
{
    ExampleParser* parser = (ExampleParser*)self;
    parser->symbols.push_back(tok);
    parser->state.push_back(std::make_unique<ParseTree>(lex));
    return true;
}

bool reduce_rule(void* self, char const* lhs, char const* const* rhs)
{
    ExampleParser* parser = (ExampleParser*)self;
    auto node = std::make_unique<ParseTree>(lhs);
    std::vector<decltype(node)> children;
    for (char const* const* p = rhs; *p; ++p) {
        parser->symbols.pop_back();
        children.push_back(std::move(parser->state.back()));
        parser->state.pop_back();
    }
    while (!children.empty()) {
        node->children.push_back(std::move(children.back()));
        children.pop_back();
    }
    parser->symbols.push_back(lhs);
    parser->state.push_back(std::move(node));
    return true;
}

auto get_shift_handler(void* self, char const* tok)
{
    ExampleParser* parser = (ExampleParser*)self;
    auto it = parser->shift_handlers.find(tok);
    if (it != parser->shift_handlers.end()) {
        return it->second;
    }
    return decltype(it->second)(nullptr);
}

auto get_reduce_handler(void* self, char const* lhs, char const* const* rhs)
{
    ExampleParser* parser = (ExampleParser*)self;
    std::vector<std::string> rhs_vec;
    for (char const* const* p = rhs; *p; ++p) {
        rhs_vec.push_back(*p);
    }
    auto it = parser->reduce_handlers.find(std::make_pair(lhs, rhs_vec));
    if (it != parser->reduce_handlers.end()) {
        return it->second;
    }
    return decltype(it->second)(nullptr);
}

bool shift(void* self, char const* tok, char const* lex)
{
    auto handler = get_shift_handler(self, tok);
    if (handler) {
        return handler(self, tok, lex);
    }
    // TODO
    return false;
}

bool reduce(void* self, char const* lhs, char const* const* rhs)
{
    auto handler = get_reduce_handler(self, lhs, rhs);
    if (handler) {
        return handler(self, lhs, rhs);
    }
    // TODO
    return false;
}

}
