#define CATCH_CONFIG_MAIN
#include "rnd/minauto.hpp"
#include "rnd/automaton.hpp"
#include "rnd/nexpr.hpp"
#include "rnd/zsymbols.hpp"
#include <catch2/catch.hpp>
#include <tuple>

using namespace rnd;

bool is_equal(const Minimized& m, const Minimized& n)
{
    if (std::tie(m.accepts, m.error) != std::tie(n.accepts, n.error)) {
        return false;
    }
    if (m.states.size() != n.states.size()) {
        return false;
    }
    for (const auto& [q, dq]: m.states) {
        if (dq.size() != n.states.at(q).size()) {
            return false;
        }
        for (const auto& [a, r]: n.states.at(q)) {
            if (r != dq.at(a)) {
                return false;
            }
        }
    }
    return true;
}

TEST_CASE("identities involving epsilon the empty set", "[nexpr]")
{
    SECTION("result = identity") {
        auto actual = Minimized(Automaton(GENERATE(
            alternate(NExpr(ZRange('a')), NExpr()),
            alternate(NExpr(), NExpr(ZRange('a'))),
            concatenate(NExpr(ZRange('a')), epsilon()),
            concatenate(epsilon(), NExpr(ZRange('a'))))));
        Minimized expected(Automaton(NExpr(ZRange('a'))));
        REQUIRE(is_equal(actual, expected));
    }

    SECTION("result = epsilon") {
        auto actual = Minimized(Automaton(GENERATE(closure(NExpr()), closure(epsilon()))));
        auto expected = Minimized(Automaton(epsilon()));
        REQUIRE(is_equal(actual, expected));
    }

    SECTION("result = NExpr()") {
        auto actual = Minimized(Automaton(GENERATE(
            concatenate(NExpr(ZRange('a')), NExpr()),
            concatenate(NExpr(), NExpr(ZRange('a'))))));
        auto expected = Minimized(Automaton(NExpr()));
        CHECK(is_equal(actual, expected));
    }
}
