#pragma once
#include <algorithm>
#include <utility>
#include <vector>
#include <tuple>

namespace lr1
{

template <class Symbol>
class Item {
public:
    using Sentence = typename std::vector<Symbol>;
    using Rule = typename std::pair<Symbol, Sentence>;

    Symbol lhs;
    Sentence before;
    Sentence after;
    Symbol lookahead;

    Item() {}
    Item(const Symbol& lhs, const Sentence& before,
            const Sentence& after, const Symbol& lookahead)
    {
        this->lhs = lhs;
        this->before = before;
        this->after = after;
        this->lookahead = lookahead;
    }

    bool operator<(const Item<Symbol>& rhs) const 
    {
        return std::tie(lhs, before, after, lookahead) 
            < std::tie(rhs.lhs, rhs.before, rhs.after, rhs.lookahead);
    }

    bool operator==(const Item<Symbol>& other) const
    {
        return !(*this < other || other < *this);
    }

    Item<Symbol> shifted() const 
    {
        // creates copy with shifted dot
        // assume after is nonempty
        if (after.empty()) {
            throw "can't shift with no symbols after the dot";
        }
        Sentence before = this->before;
        before.push_back(this->after.front());
        Sentence after(this->after.begin() + 1, this->after.end());
        return Item<Symbol>(lhs, before, after, lookahead);
    }

    bool is_reduce() const 
    {
        return after.empty();
    }

    Rule rule() const
    {
        Sentence rhs;
        std::remove_copy(before.begin(), before.end(), 
                std::back_inserter(rhs), Symbol());
        std::remove_copy(after.begin(), after.end(), 
                std::back_inserter(rhs), Symbol());
        return Rule(lhs, rhs);
    }

    Symbol symbol_after_dot() const
    {
        if (after.empty()) {
            return Symbol();
        }
        return after.front();
    }
};

} // end namespace
