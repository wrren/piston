#include <piston/file/pe.h>

namespace piston
{
    pe_file::pe_file()
    {}

    pe_file::pe_file(const path& path) :
    file(path)
    {}

    bool pe_file::is_valid() const
    {
        return m_dos_header.is_valid();
    }

    bool pe_file::deserialize(std::istream& stream)
    {
        return m_dos_header.deserialize(stream);
    }
}