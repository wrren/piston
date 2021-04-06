#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <piston/process/memory/region.h>

TEST_CASE("Memory Region", "[memory]")
{
    Piston::MemoryRegion region_a(0x1234, 4);
    Piston::MemoryRegion region_b(0x1236, 10);
    Piston::MemoryRegion region_c(0x123B, 4);

    SECTION("Checking for Overlaps")
    {
        REQUIRE(region_a.OverlapsWith(region_b));
        REQUIRE(region_b.OverlapsWith(region_c));
        REQUIRE_FALSE(region_a.OverlapsWith(region_c));
    }
}