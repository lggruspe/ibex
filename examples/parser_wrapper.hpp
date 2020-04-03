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
        return false;
    }
    return it->second(self, lex);
}

template <class T>
bool reduce(void* arg, char const* lhs, char const* const* rhs)
{
    T* self = (T*)arg;
    std::vector<std::string> rhs_vec;
    for (auto w = rhs; *w; ++w) {
        rhs_vec.push_back(*w);
    }
    auto it = self->reduce.find({lhs, rhs_vec});
    if (it == self->reduce.end()) {
        return false;
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
    std::map<std::pair<std::string, std::vector<std::string>>, ReduceHandler> reduce;

    T* parse(char const* text)
    {
        ok = ::parse(text, ::shift<T>, ::reduce<T>, this);
        return (T*)this;
    }
};
