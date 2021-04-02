#include <piston/file/file.h>
#include <piston/core/format.h>
#include <fstream>

namespace piston
{
    file::file()
    {}

    file::file(const path& path) :
    m_path(path)
    {}

    const path& file::get_path() const
    {
        return m_path;
    }

    file& file::operator=(const path& path)
    {
        m_path = path;
        return *this;
    }

    bool file::read()
    {
        if(!std::filesystem::exists(m_path))
        {
            return false;
        }

        std::fstream stream(m_path, std::fstream::in);
        if(!stream.is_open())
        {
            return false;
        }

        return deserialize(stream);
    }

    bool file::write()
    {
        if(!std::filesystem::exists(m_path))
        {
            return false;
        }

        std::fstream stream(m_path);
        if(!stream.is_open())
        {
            return false;
        }

        return serialize(stream);
    }
}