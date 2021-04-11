#include <piston/core/stream.h>
#include <cstring>

namespace Piston
{
    Stream::PositionType Stream::GetPosition() const
    {
        return mPosition;
    }

    Stream& Stream::Seek(Stream::PositionType NewPosition)
    {
        mPosition = NewPosition;
        mGood = mPosition <= End();
        return *this;
    }

    bool Stream::Good() const
    {
        return mGood;
    }

    Stream& Stream::operator<<(const char* InputString)
    {
        PositionType AllowedCapacity = (End() - mPosition) - sizeof(uint32_t);
        uint32_t StringLength = static_cast<uint32_t>(strnlen_s(InputString, AllowedCapacity));

        WriteBuffer(reinterpret_cast<const byte*>(&StringLength), sizeof(uint32_t));
        WriteBuffer(reinterpret_cast<const byte*>(InputString), StringLength);

        return *this;
    }

    Stream& Stream::operator<<(const String& InputString)
    {
        PositionType AllowedCapacity = (End() - mPosition) - sizeof(uint32_t);
        uint32_t StringLength = static_cast<uint32_t>(InputString.length());

        WriteBuffer(reinterpret_cast<const byte*>(&StringLength), sizeof(uint32_t));
        WriteBuffer(reinterpret_cast<const byte*>(InputString.c_str()), StringLength);

        return *this;
    }

    Stream& Stream::operator>>(String& OutputString)
    {
        uint32_t StringLength;

        ReadBuffer(reinterpret_cast<byte*>(&StringLength), sizeof(uint32_t));

        if(mGood)
        {
            std::unique_ptr<char> StringBuffer(new char[StringLength]);
            ReadBuffer(reinterpret_cast<byte*>(OutputString.data()), StringLength);
        }

        return *this;
    }

    bool Stream::WriteBuffer(const byte* Data, size_t DataSize)
    {
        if(mPosition + DataSize > End())
        {
            mGood = false;
        }
        else
        {
            mGood = mGood && Write(Data, DataSize);
            mPosition += DataSize;
        }

        return mGood;
    }

    bool Stream::ReadBuffer(byte* Data, size_t DataSize)
    {
        if(mPosition + DataSize > End())
        {
            mGood = false;
        }
        else
        {
            mGood = mGood && Read(Data, DataSize);
            mPosition += DataSize;
        }

        return mGood;
    }
}