#include <piston/core/byte_stream.h>

namespace Piston
{
    ByteStream::ByteBuffer::ByteBuffer(byte* Data, size_t Size)
    {
        setg((char*) Data, (char*) Data, (char*) Data + Size);
        setp((char*) Data, (char*) Data, (char*) Data + Size);
    }

    ByteStream::ByteStream(byte* Data, size_t Size) :
    mBuffer(Data, Size),
    std::iostream(&mBuffer)
    {}
}