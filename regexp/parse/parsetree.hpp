#pragma once
#include <rnd.h>
#include <cstdint>
#include <memory>
#include <utility>
#include <variant>
#include <vector>

namespace parse {
    enum class Label { Start, Expr, Term, Factor, Value, Error, Empty, Pipe,
        Star, Lparen, Rparen, Dot, Question, Plus, Interval, Symbol
    };

    struct Tree {
        Label label;
        rnd_expr *value;
        std::vector<std::unique_ptr<Tree>> children;
    };
}
