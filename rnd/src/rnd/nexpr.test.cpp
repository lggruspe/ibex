#define CATCH_CONFIG_MAIN
#include "rnd/nexpr.hpp"
#include "rnd/zsymbols.hpp"
#include <catch2/catch.hpp>
#include <cstdint>
#include <limits>

// NOTE regular operaitions are checked in automaton.test.cpp

using namespace rnd;

constexpr uint32_t max = std::numeric_limits<uint32_t>::max();

TEST_CASE("NExpr() = empty set", "[nexpr]")
{
    NExpr expr;
    REQUIRE(expr.states.size() == 2);
    REQUIRE(expr.states.at(0).empty());
    REQUIRE(expr.states.at(1).empty());
}

TEST_CASE("NExpr(a)", "[nexpr]")
{
    SECTION("epsilon") {
        auto eps = GENERATE(
            epsilon(),
            NExpr(ZRange(42, 42)),
            NExpr(ZRange()),
            NExpr(ZRange(max)));
        REQUIRE(eps.symbols.points.size() == 2);
        REQUIRE(eps.states.size() == 2);
        REQUIRE(eps.states.at(0).size() == 1);
        REQUIRE(eps.states.at(1).empty());
        for (const auto& [a, R]: eps.states.at(0)) {
            REQUIRE(a.is_empty());
            REQUIRE(R.size() == 1);
            REQUIRE(R.count(1));
        }
    }

    SECTION("nonepsilon") {
        auto a = GENERATE(as<uint32_t>{}, 0, 42, max-1);
        NExpr expr(a);
        REQUIRE(expr.symbols.points.size() == (a == 42 ? 4 : 3));
        REQUIRE(expr.symbols.points.count(0));
        REQUIRE(expr.symbols.points.count(max));
        REQUIRE(expr.symbols.points.count(a));
        REQUIRE(expr.symbols.points.count(a+1));
        REQUIRE(expr.states.size() == 2);
        REQUIRE(expr.states.at(1).empty());
        REQUIRE(expr.states.at(0).size() == 1);
        for (const auto& [a, R]: expr.states.at(0)) {
            REQUIRE(!a.is_empty());
            REQUIRE(a.start+1 == a.end);
            REQUIRE(R.size() == 1);
            REQUIRE(R.count(1));
        }
    }
}

/*
bool is_equal(const NExpr& a, const NExpr& b)
{
    if (a.symbols.points != b.symbols.points) {
        return false;
    }
    if (a.states.size() != b.states.size()) {
        return false;
    }
    for (int q = 0; q < (int)(a.states.size()); ++q) {
        for (const auto& [a, R]: a.states.at(q)) {
            if (R.size() != b.states.at(q).at(a).size()) {
                return false;
            }
            for (const auto& r: R) {
                if (!b.states.at(q).at(a).count(r)) {
                    return false;
                }
            }
        }
    }
    return true;
}

TEST_CASE("identities involving the empty set", "[nexpr]")
{
    NExpr a(ZRange('a'));
    REQUIRE(is_equal(alternate(a, NExpr()), a));
    REQUIRE(is_equal(alternate(NExpr(), a), a));
    REQUIRE(is_equal(concatenate(a, NExpr()), NExpr()));
    REQUIRE(is_equal(concatenate(NExpr(), a), NExpr()));
    REQUIRE(is_equal(closure(NExpr()), epsilon()));
}

TEST_CASE("identities involving epsilon", "[nexpr]")
{
    NExpr a(ZRange('a'));
    REQUIRE(is_equal(concatenate(a, epsilon()), a));
    REQUIRE(is_equal(concatenate(epsilon(), a), a));
    REQUIRE(is_equal(closure(epsilon()), epsilon()));
}
*/
