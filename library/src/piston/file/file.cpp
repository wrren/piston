#include <piston/file/file.h>

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

    path& file::operator=(const path& path)
    {
        m_path = path;
    }
}