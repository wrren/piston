#include <piston/process/memory/scanner.h>
#include <catch2/catch.hpp>

TEST_CASE("Memory Scanner", "[memory]")
{
    piston::memory_region region(0x0, {
        0x1, 0x2, 0x3, 0x4, 0x5, 0x6,
        0x7, 0x8, 0x9, 0xA, 0xB, 0xC,
    });

    SECTION("Byte Value Scanner", "[memory]")
    {
        auto value_scanner = piston::memory_value_scanner<piston::byte>(0x1);
        auto results = value_scanner.scan(region);
        REQUIRE(results.size() == 1);
        REQUIRE(results[0].get_base_address() == 0x0);
    }

    SECTION("Short Value Scanner", "[memory]")
    {
        auto value_scanner = piston::memory_value_scanner<uint16_t>(0x0201);
        auto results = value_scanner.scan(region);
        REQUIRE(results.size() == 1);
        REQUIRE(results[0].get_base_address() == 0x0);
    }

    SECTION("Integral Scanner - Equals", "[memory]")
    {
        auto integral_scanner = piston::memory_integral_scanner<uint8_t>(0x6, piston::memory_integral_scanner<uint8_t>::compare_mode::COMPARE_EQUALS);
        auto results = integral_scanner.scan(region);
        REQUIRE(results.size() == 1);
        REQUIRE(results[0].get_base_address() == 0x5);
    }

    SECTION("Integral Scanner - Less Than", "[memory]")
    {
        auto integral_scanner = piston::memory_integral_scanner<uint8_t>(0x6, piston::memory_integral_scanner<uint8_t>::compare_mode::COMPARE_LESS_THAN);
        auto results = integral_scanner.scan(region);
        REQUIRE(results.size() == 5);
        REQUIRE(results[0].get_base_address() == 0x0);
        REQUIRE(results[1].get_base_address() == 0x1);
        REQUIRE(results[2].get_base_address() == 0x2);
        REQUIRE(results[3].get_base_address() == 0x3);
        REQUIRE(results[4].get_base_address() == 0x4);
    }

    SECTION("Integral Scanner - Greater Than", "[memory]")
    {
        auto integral_scanner = piston::memory_integral_scanner<uint8_t>(0x6, piston::memory_integral_scanner<uint8_t>::compare_mode::COMPARE_GREATER_THAN);
        auto results = integral_scanner.scan(region);
        REQUIRE(results.size() == 6);
        REQUIRE(results[0].get_base_address() == 0x6);
        REQUIRE(results[1].get_base_address() == 0x7);
        REQUIRE(results[2].get_base_address() == 0x8);
        REQUIRE(results[3].get_base_address() == 0x9);
        REQUIRE(results[4].get_base_address() == 0xA);
        REQUIRE(results[5].get_base_address() == 0xB);
    }

    SECTION("Integral Scanner - Less Than Or Equal", "[memory]")
    {
        auto integral_scanner = piston::memory_integral_scanner<uint8_t>(0x6, piston::memory_integral_scanner<uint8_t>::compare_mode::COMPARE_LESS_THAN_OR_EQUAL);
        auto results = integral_scanner.scan(region);
        REQUIRE(results.size() == 6);
        REQUIRE(results[0].get_base_address() == 0x0);
        REQUIRE(results[1].get_base_address() == 0x1);
        REQUIRE(results[2].get_base_address() == 0x2);
        REQUIRE(results[3].get_base_address() == 0x3);
        REQUIRE(results[4].get_base_address() == 0x4);
        REQUIRE(results[5].get_base_address() == 0x5);

    }

    SECTION("Integral Scanner - Greater Than Or Equal", "[memory]")
    {
        auto integral_scanner = piston::memory_integral_scanner<uint8_t>(0x6, piston::memory_integral_scanner<uint8_t>::compare_mode::COMPARE_GREATER_THAN_OR_EQUAL);
        auto results = integral_scanner.scan(region);
        REQUIRE(results.size() == 7);
        REQUIRE(results[0].get_base_address() == 0x5);
        REQUIRE(results[1].get_base_address() == 0x6);
        REQUIRE(results[2].get_base_address() == 0x7);
        REQUIRE(results[3].get_base_address() == 0x8);
        REQUIRE(results[4].get_base_address() == 0x9);
        REQUIRE(results[5].get_base_address() == 0xA);
        REQUIRE(results[6].get_base_address() == 0xB);
    }
}