#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <piston/process/memory/region.h>

TEST_CASE("Memory Region", "[memory]")
{
    piston::memory_region region_a(0x1234, {0x1, 0x2, 0x3, 0x4, 0x5, 0x6});
    piston::memory_region region_b(0x1236, {0x1, 0x2, 0x3, 0x4, 0x5, 0x6});
    piston::memory_region region_c(0x123B, {0x1, 0x2, 0x3, 0x4, 0x5, 0x6});

    SECTION("Checking for Overlaps")
    {
        REQUIRE(region_a.overlaps_with(region_b));
        REQUIRE(region_b.overlaps_with(region_c));
        REQUIRE_FALSE(region_a.overlaps_with(region_c));
    }

    SECTION("Diffing Overlapping Regions")
    {
        auto diff = region_a.diff(region_b);
        REQUIRE(diff.size() == 1);
        REQUIRE(diff[0].get_size() == 4);
        REQUIRE(diff[0].get_base_address() == 0x1236);
    }

    SECTION("Diffing Non-Overlapping Regions")
    {
        auto diff = region_a.diff(region_c);
        REQUIRE(diff.size() == 0);
    }
}