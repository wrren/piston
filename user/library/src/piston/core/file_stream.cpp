#include <piston/core/file_stream.h>

namespace Piston
{
    FileStream::FileStream(const Path& FilePath, const char* OpenMode) :
    mPath(FilePath)
    {
        fopen_s(&mFileHandle, mPath.string().c_str(), OpenMode);
    }

    Stream::PositionType FileStream::End() const
    {
        return std::numeric_limits<uint32_t>::max();
    }

    bool FileStream::Write(const byte* Data, size_t DataSize)
    {
        if(mFileHandle == nullptr || ferror(mFileHandle) != 0)
        {
            return false;
        }

        if(ftell(mFileHandle) != GetPosition())
        {
            if(fseek(mFileHandle, static_cast<long>(GetPosition()), SEEK_SET))
            {
                return false;
            }
        }

        if(fwrite(Data, sizeof(byte), DataSize, mFileHandle) != DataSize)
        {
            return false;
        }

        return true;
    }

    bool FileStream::Read(byte* Data, size_t DataSize) const
    {
        if(mFileHandle == nullptr || ferror(mFileHandle) != 0)
        {
            return false;
        }

        if(ftell(mFileHandle) != GetPosition())
        {
            if(fseek(mFileHandle, static_cast<long>(GetPosition()), SEEK_SET))
            {
                return false;
            }
        }

        if(fread(Data, sizeof(byte), DataSize, mFileHandle) != DataSize)
        {
            return false;
        }

        return true;
    }

    FileStream::~FileStream()
    {
        if(mFileHandle)
        {
            fclose(mFileHandle);
            mFileHandle = nullptr;
        }
    }
}