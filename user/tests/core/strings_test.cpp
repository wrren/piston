#include <catch2/catch.hpp>
#include <piston/core/strings.h>

TEST_CASE("Strings", "[core]")
{
    Piston::WideString WString = L"Wide String";
    REQUIRE(Piston::Strings::WideStringToString(WString) == "Wide String");
}