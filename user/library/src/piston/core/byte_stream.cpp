#include <piston/core/byte_stream.h>

namespace Piston
{
    ByteStream::ByteStream(byte* Buffer, Stream::PositionType BufferSize) :
    mBuffer(Buffer),
    mSize(BufferSize)
    {}

    Stream::PositionType ByteStream::End() const
    {
        return mSize;
    }

    bool ByteStream::Write(const byte* Data, size_t DataSize)
    {
        auto Position = GetPosition();

        if(Position + DataSize > mSize)
        {
            return false;
        }

        memcpy(mBuffer + Position, Data, DataSize);

        return true;
    }

    bool ByteStream::Read(byte* Data, size_t DataSize) const
    {
        auto Position = GetPosition();

        if(Position + DataSize > mSize)
        {
            return false;
        }

        memcpy(Data, mBuffer + Position, DataSize);

        return true;
    }
}