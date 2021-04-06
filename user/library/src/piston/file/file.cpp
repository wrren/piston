#include <piston/file/file.h>
#include <piston/core/format.h>
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

        std::fstream stream(mPath, std::fstream::in);
        if(!stream.is_open())
        {
            return false;
        }

        return Deserialize(stream);
    }

    bool File::Write()
    {
        if(!std::filesystem::exists(mPath))
        {
            return false;
        }

        std::fstream stream(mPath);
        if(!stream.is_open())
        {
            return false;
        }

        return Serialize(stream);
    }
}