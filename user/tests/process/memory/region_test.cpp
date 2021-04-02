#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <piston/process/memory/region.h>

TEST_CASE("Memory Region", "[memory]")
{
    piston::memory_region region_a(0x1234, 0x1238);
    piston::memory_region region_b(0x1236, 0x123A);
    piston::memory_region region_c(0x123B, 0x123F);

    SECTION("Checking for Overlaps")
    {
        REQUIRE(region_a.overlaps_with(region_b));
        REQUIRE(region_b.overlaps_with(region_c));
        REQUIRE_FALSE(region_a.overlaps_with(region_c));
    }
}