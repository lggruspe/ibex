#define CATCH_CONFIG_MAIN
#include "rnd/zsymbols.hpp"
#include <catch2/catch.hpp>
#include <cstdint>
#include <sys/wait.h>
#include <unistd.h>
#include <limits>
#include <vector>

using namespace rnd;

constexpr auto max = std::numeric_limits<uint32_t>::max();

TEST_CASE("ZRange() = [max, max)", "[zrange]")
{
    ZRange empty;
    REQUIRE(empty.start == std::numeric_limits<uint32_t>::max());
    REQUIRE(empty.start == empty.end);
    REQUIRE(empty.is_empty());
}

TEST_CASE("ZRange(a) = [a, a+1) unless a = max", "[zrange]")
{
    ZRange cases[] = {
        ZRange(0),
        ZRange(1),
        ZRange(42),
        ZRange(max-1),
        ZRange(max),
        ZRange(-1)
    };
    for (const auto& r: cases) {
        const auto& [start, end] = r;
        if (start == max) {
            REQUIRE(start == end);
        } else {
            REQUIRE(start+1 == end);
            REQUIRE(!r.is_empty());
        }
    }
    REQUIRE(cases[5].start == max);
    REQUIRE(cases[5].end == max);
    REQUIRE(cases[5].is_empty());
}

TEST_CASE("ZRange(a, b) = [a, b)")
{
    std::vector<ZRange> cases;

    SECTION("empty") {
        uint32_t values[] = {0, 1, 42, max-1, max};
        for (const auto& v: values) {
            cases.push_back(ZRange(v, v));
        }
        for (int i = 0; i < 5; ++i) {
            REQUIRE(cases[i].start == cases[i].end);
            REQUIRE(cases[i].start == values[i]);
            REQUIRE(cases[i].is_empty());
        }
    }

    SECTION("nonempty") {
        cases.push_back(ZRange(0,1));
        cases.push_back(ZRange(1, max-1));
        cases.push_back(ZRange(42, 44));
        cases.push_back(ZRange(max-1, max));
        for (const auto& r: cases) {
            REQUIRE(!r.is_empty());
        }
    }
}

/*
TEST_CASE("ZRange(a, b) where a > b", "[zrange]")
{
    SECTION("input 1") { ZRange(-11, 0); }
    SECTION("input 2") { ZRange(1, 0); }
    SECTION("input 3") { ZRange(max-1, 1); }
    SECTION("input 4") { ZRange(max, max-1); }
    SECTION("input 5") { ZRange(max, max+1); }
}
*/

/*
int main()
{
    ZPartition p = {
        ZRange(1, 5),
        ZRange(2, 4),
        ZRange(1, 3),
        ZRange(10, 11),
        ZRange(1, 11),
        ZRange(17, 17),
    };

    ZPartition q = {
        ZRange(1, 5),
        ZRange(2, 4),
        ZRange(1, 3),
        ZRange(4, 9),
        ZRange(5, 18),
        ZRange(17, 1230),
    };

    auto r = p.combined(q);
    for (const auto& [a, b]: r.to_set()) {
        std::cout << "[" << a << ", " << b << ")" << std::endl;
    }
    for (const auto& x: r.points) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    const auto& cover = r.cover(ZRange(15, 15));
    for (const auto& [a, b]: cover) {
        std::cout << "[" << a << ", " << b << ")" << std::endl;
    }
}
*/
