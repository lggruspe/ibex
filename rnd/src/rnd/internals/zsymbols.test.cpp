#define CATCH_CONFIG_MAIN
#include "zsymbols.hpp"
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
                auto cover = p.cover(r);
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
    ZPartition p = {
        ZRange(0, max),
        ZRange(1, max),
        ZRange(0, max-1),
        ZRange(),
        ZRange(42, 44)
    };

    SECTION("number of points - 1 = number of ranges") {
        REQUIRE(p.points.size() == 6);
        REQUIRE(p.points.count(0));
        REQUIRE(p.points.count(1));
        REQUIRE(p.points.count(42));
        REQUIRE(p.points.count(44));
        REQUIRE(p.points.count(max-1));
        REQUIRE(p.points.count(max));
        const auto& ranges = p.to_set();
        REQUIRE(ranges.size() == 5);
        ZRange expected[] = {
            ZRange(0),
            ZRange(1, 42),
            ZRange(42, 44),
            ZRange(44, max-1),
            ZRange(max-1)
        };
        int i = 0;
        for (const auto& r: ranges) {
            REQUIRE(r.start == expected[i].start);
            REQUIRE(r.end == expected[i].end);
            ++i;
        }

        SECTION("insert(ran)") {
            REQUIRE(p.points.size() == 6);
            SECTION("when ran is empty") {
                p.insert(ZRange());
                p.insert(ZRange(0,0));
                p.insert(ZRange(42,42));
                REQUIRE(p.points.size() == 6);
            }
            SECTION("when ran.start and ran.end are in p.points") {
                p.insert(ZRange(42, 44));
                REQUIRE(p.points.size() == 6);
            }
            SECTION("when only ran.start is in p.points") {
                auto ran = GENERATE(ZRange(42, 43), ZRange(42, 45));
                p.insert(ran);
                REQUIRE(p.points.size() == 7);
            }
            SECTION("when only ran.end is in p.points") {
                auto ran = GENERATE(ZRange(41,44), ZRange(43,44));
                p.insert(ran);
                REQUIRE(p.points.size() == 7);
            }
            SECTION("when ran.start and ran.end are not in p.points") {
                p.insert(ZRange(41, 45));
                REQUIRE(p.points.size() == 8);
            }
        }

        SECTION("combined") {
            ZPartition q = {
                {0,10},
                {1,9},
                {2,8},
                {3,7},
                {4,6},
                {5,5},
            };
            REQUIRE(q.points.size() == 11);
            REQUIRE(q.to_set().size() == 10);
            auto s = p.combined(q);
            REQUIRE(s.points.size() == 14);
            REQUIRE(s.to_set().size() == 13);
        }
    }

    SECTION("cover") {
        SECTION("cover(ran) is empty when ran is empty") {
            ZRange cases[] = {
                ZRange(0,0),
                ZRange(1,1),
                ZRange(42,42),
                ZRange(max-1,max-1),
                ZRange(max,max),
                ZRange()
            };
            for (const auto& r: cases) {
                REQUIRE(r.is_empty());
                REQUIRE(p.cover(r).empty());
            }
        }

        SECTION("cover(ran) general case") {
            SECTION("example 1") {
                auto cover = p.cover(ZRange(0,42));
                REQUIRE(cover.size() == 2);
                REQUIRE(cover.count(ZRange(0)));
                REQUIRE(cover.count(ZRange(1,42)));
            }
            SECTION("example 2") {
                auto cover = p.cover(ZRange(42,45));
                REQUIRE(cover.size() == 2);
                REQUIRE(cover.count(ZRange(42,44)));
                REQUIRE(cover.count(ZRange(44,max-1)));
            }
            SECTION("example 3") {
                auto cover = p.cover(ZRange(44,max-1));
                REQUIRE(cover.size() == 1);
                REQUIRE(cover.count(ZRange(44,max-1)));
            }
            SECTION("example 4") {
                auto cover = p.cover(ZRange(max-1,max));
                REQUIRE(cover.size() == 1);
                REQUIRE(cover.count(ZRange(max-1)));
            }
            SECTION("example 5") {
                auto cover = p.cover(ZRange(0,1));
                REQUIRE(cover.size() == 1);
                REQUIRE(cover.count(ZRange(0)));
            }
            SECTION("example 6") {
                auto cover = p.cover(ZRange(1,42));
                REQUIRE(cover.size() == 1);
                REQUIRE(cover.count(ZRange(1,42)));
            }
            SECTION("example 7") {
                auto cover = p.cover(ZRange(42,max));
                REQUIRE(cover.size() == 3);
                REQUIRE(cover.count(ZRange(42,44)));
                REQUIRE(cover.count(ZRange(44,max-1)));
                REQUIRE(cover.count(ZRange(max-1)));
            }
            SECTION("example 8") {
                REQUIRE(p.cover(ZRange(0,max)).size() == p.points.size() - 1);
            }
        }
    }
}
