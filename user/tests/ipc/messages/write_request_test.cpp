#include <catch2/catch.hpp>
#include <piston/core/byte_stream.h>
#include <piston/ipc/messages/write_request.h>

TEST_CASE("Write Request", "[ipc::messages]")
{
    Piston::byte Buffer[2048];
    Piston::ByteStream Stream(Buffer, 2048);

    uint32_t Value = 100;

    Piston::IPC::WriteRequest In(1234, reinterpret_cast<byte*>(&Value), sizeof(Value));
    In.Serialize(Stream);

    Stream.Seek(0);

    Piston::IPC::WriteRequest Out;
    Out.Deserialize(Stream);

    REQUIRE(In.GetTargetAddress() == Out.GetTargetAddress());
    REQUIRE(*(reinterpret_cast<uint32_t*>(Out.GetData())) == 100);
}