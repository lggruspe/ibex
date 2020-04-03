#pragma once
#include "parser.h"
#include <map>
#include <string>
#include <vector>

template <class T>
bool shift(void* arg, char const* tok, char const* lex)
{
    T* self = (T*)arg;
    auto it = self->shift.find(tok);
    if (it == self->shift.end()) {
        return self->default_shift(tok, lex);
    }
    return it->second(self, lex);
}

template <class T>
bool reduce(void* arg, char const* lhs, char const* rhs, int len)
{
    T* self = (T*)arg;
    std::string query = rhs;
    query = std::string(lhs) + (query.empty() ? " ->" : " -> " + query);
    auto it = self->reduce.find(query);
    if (it == self->reduce.end()) {
        return self->default_reduce(lhs, rhs, len);
    }
    return it->second(self);
}

template <class T>
class BaseParser {
public:
    using ShiftHandler = bool (*)(T*, char const*);
    using ReduceHandler = bool (*)(T*);

    bool ok = false;
    std::map<std::string, ShiftHandler> shift;
    std::map<std::string, ReduceHandler> reduce;

    T* parse(char const* text)
    {
        ok = ::parse(text, ::shift<T>, ::reduce<T>, this);
        return (T*)this;
    }

    bool default_shift(const std::string& tok, const std::string& lex)
    {
        (void)tok;
        (void)lex;
        return false;
    }

    bool default_reduce(const std::string& lhs, const std::string& rhs, int len)
    {
        (void)lhs;
        (void)rhs;
        (void)len;
        return false;
    }
};
