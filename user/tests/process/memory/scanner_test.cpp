#include <piston/process/memory/scanner.h>
#include <catch2/catch.hpp>

TEST_CASE("Memory Scanner", "[memory]")
{
    Piston::byte region[] = {
        0x1, 0x2, 0x3, 0x4, 0x5, 0x6,
        0x7, 0x8, 0x9, 0xA, 0xB, 0xC,
    };

    SECTION("Byte Value Scanner", "[memory]")
    {
        auto value_scanner = Piston::MemoryValueScanner<Piston::byte>(0x1);
        auto results = value_scanner.Scan(&region[0], sizeof(region));
        REQUIRE(results.size() == 1);
        REQUIRE(results[0] == 0);
    }

    SECTION("Short Value Scanner", "[memory]")
    {
        auto value_scanner = Piston::MemoryValueScanner<uint16_t>(0x0201);
        auto results = value_scanner.Scan(&region[0], sizeof(region));
        REQUIRE(results.size() == 1);
        REQUIRE(results[0] == 0);
    }

    SECTION("Integral Scanner - Equals", "[memory]")
    {
        auto integral_scanner = Piston::MemoryIntegralScanner<uint8_t>(0x6, Piston::MemoryIntegralScanner<uint8_t>::CompareMode::COMPARE_EQUALS);
        auto results = integral_scanner.Scan(&region[0], sizeof(region));
        REQUIRE(results.size() == 1);
        REQUIRE(results[0] == 5);
    }

    SECTION("Integral Scanner - Less Than", "[memory]")
    {
        auto integral_scanner = Piston::MemoryIntegralScanner<uint8_t>(0x6, Piston::MemoryIntegralScanner<uint8_t>::CompareMode::COMPARE_LESS_THAN);
        auto results = integral_scanner.Scan(&region[0], sizeof(region));
        REQUIRE(results.size() == 5);
        REQUIRE(results[0] == 0);
        REQUIRE(results[1] == 1);
        REQUIRE(results[2] == 2);
        REQUIRE(results[3] == 3);
        REQUIRE(results[4] == 4);
    }

    SECTION("Integral Scanner - Greater Than", "[memory]")
    {
        auto integral_scanner = Piston::MemoryIntegralScanner<uint8_t>(0x6, Piston::MemoryIntegralScanner<uint8_t>::CompareMode::COMPARE_GREATER_THAN);
        auto results = integral_scanner.Scan(&region[0], sizeof(region));
        REQUIRE(results.size() == 6);
        REQUIRE(results[0] == 6);
        REQUIRE(results[1] == 7);
        REQUIRE(results[2] == 8);
        REQUIRE(results[3] == 9);
        REQUIRE(results[4] == 10);
        REQUIRE(results[5] == 11);
    }

    SECTION("Integral Scanner - Less Than Or Equal", "[memory]")
    {
        auto integral_scanner = Piston::MemoryIntegralScanner<uint8_t>(0x6, Piston::MemoryIntegralScanner<uint8_t>::CompareMode::COMPARE_LESS_THAN_OR_EQUAL);
        auto results = integral_scanner.Scan(&region[0], sizeof(region));
        REQUIRE(results.size() == 6);
        REQUIRE(results[0] == 0);
        REQUIRE(results[1] == 1);
        REQUIRE(results[2] == 2);
        REQUIRE(results[3] == 3);
        REQUIRE(results[4] == 4);
        REQUIRE(results[5] == 5);

    }

    SECTION("Integral Scanner - Greater Than Or Equal", "[memory]")
    {
        auto integral_scanner = Piston::MemoryIntegralScanner<uint8_t>(0x6, Piston::MemoryIntegralScanner<uint8_t>::CompareMode::COMPARE_GREATER_THAN_OR_EQUAL);
        auto results = integral_scanner.Scan(&region[0], sizeof(region));
        REQUIRE(results.size() == 7);
        REQUIRE(results[0] == 5);
        REQUIRE(results[1] == 6);
        REQUIRE(results[2] == 7);
        REQUIRE(results[3] == 8);
        REQUIRE(results[4] == 9);
        REQUIRE(results[5] == 10);
        REQUIRE(results[6] == 11);
    }
}