#include <piston/file/file.h>
#include <piston/core/format.h>
#include <piston/core/file_stream.h>
#include <fstream>

namespace Piston
{
    File::File()
    {}

    File::File(const Path& path) :
    mPath(path)
    {}

    const Path& File::GetPath() const
    {
        return mPath;
    }

    File& File::operator=(const Path& path)
    {
        mPath = path;
        return *this;
    }

    bool File::Read()
    {
        if(!std::filesystem::exists(mPath))
        {
            return false;
        }

        FileStream Stream(mPath, "r");

        return Deserialize(Stream);
    }

    bool File::Write()
    {
        if(!std::filesystem::exists(mPath))
        {
            return false;
        }

        FileStream Stream(mPath, "w");

        return Serialize(Stream);
    }
}