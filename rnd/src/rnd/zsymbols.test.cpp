#define CATCH_CONFIG_MAIN
#include "rnd/zsymbols.hpp"
#include <catch2/catch.hpp>
#include <cstdint>
#include <limits>
#include <vector>

using namespace rnd;

constexpr auto max = std::numeric_limits<uint32_t>::max();

TEST_CASE("ZRange() = [max, max)", "[zrange]")
{
    ZRange empty;
    REQUIRE(empty.start == max);
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

TEST_CASE("ZRange(a, b) where a > b", "[zrange]")
{
    REQUIRE_THROWS(ZRange(-11, 0));
    REQUIRE_THROWS(ZRange(1, 0));
    REQUIRE_THROWS(ZRange(max-1, 1));
    REQUIRE_THROWS(ZRange(max, max-1));
    REQUIRE_THROWS(ZRange(max, max+1));
}

TEST_CASE("ZPartition() = {0, max}", "[zpartition]")
{
    ZPartition p;
    REQUIRE(p.points.size() == 2);
    REQUIRE(p.points.count(0));
    REQUIRE(p.points.count(max));

    SECTION("default partition contains one range") {
        auto ranges = p.to_set();
        REQUIRE(ranges.size() == 1);
        const auto& ran = ranges.begin();
        REQUIRE(ran->start == 0);
        REQUIRE(ran->end == max);

        std::vector<ZRange> cases;

        SECTION("cover(ran) = [0, max) in the general case") {
            cases.push_back(ZRange(0));
            cases.push_back(ZRange(max-1, max));
            cases.push_back(ZRange(0, max));
            cases.push_back(ZRange(42, 44));
            for (const auto& r: cases) {
                REQUIRE(!r.is_empty());
                const auto& cover = p.cover(r);
                REQUIRE(cover.size() == 1);
                REQUIRE(cover.begin()->start == 0);
                REQUIRE(cover.begin()->end == max);
            }
        }

        SECTION("cover(ran) = empty when ran is empty") {
            cases.push_back(ZRange(0, 0));
            cases.push_back(ZRange(max, max));
            cases.push_back(ZRange(max));
            cases.push_back(ZRange());
            cases.push_back(ZRange(42, 42));
            for (const auto& r: cases) {
                REQUIRE(r.is_empty());
                REQUIRE(p.cover(r).empty());
            }
        }
    }
}

TEST_CASE("ZPartition({...})", "[zpartition]")
{

}

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
