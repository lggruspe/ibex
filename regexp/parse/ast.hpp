#pragma once
#include <rnd.h>
#include "../../rnd/src/nexpr.hpp"
#include <cstdint>
#include <memory>
#include <utility>
#include <variant>
#include <vector>

namespace ast {
    enum class Label { Start, Expr, Term, Factor, Value, Error, Empty, Pipe,
        Star, Lparen, Rparen, Dot, Question, Plus, Interval, Symbol
    };

    struct Tree {
        Label label;
        rnd_expr *value;
        std::vector<std::unique_ptr<Tree>> children;
    };
}
