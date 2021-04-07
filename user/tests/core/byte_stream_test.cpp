#include <catch2/catch.hpp>
#include <piston/core/byte_stream.h>

TEST_CASE("Byte Stream", "[core]")
{
    Piston::byte Buffer[2048];
    Piston::ByteStream Stream(Buffer, 2048);

    uint32_t In = 1234, Out = 0;

    Stream << In;
    Stream.seekg(0, Stream.beg);
    Stream >> Out;

    REQUIRE(In == Out);
}