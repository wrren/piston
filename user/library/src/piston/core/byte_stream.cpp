#include <piston/core/byte_stream.h>

namespace Piston
{
    ByteStream::ByteBuffer::ByteBuffer(const byte* Data, size_t Size)
    {
        setg(Data, Data, Data + Size);
    }

    ByteStream::ByteStream(const byte* Data, size_t Size) :
    std::istream(&mBuffer),
    std::ostream(&mBuffer),
    mBuffer(Data, Size)
    {
        rdbuf(&mBuffer);
    }
}