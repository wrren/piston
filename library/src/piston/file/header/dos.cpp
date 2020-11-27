#include <piston/file/header/dos.h>
#include <cstdlib>

namespace piston
{
    const uint16_t dos_header::k_magic = 0x5A4D;

    dos_header::dos_header() :
    e_magic(m_data, 0),
    e_ifanew(m_data, 60)
    {
        memset(m_data, 0, DOS_HEADER_SIZE);
    }

    bool dos_header::deserialize(std::istream& stream)
    {
        return stream.read(reinterpret_cast<char*>(m_data), DOS_HEADER_SIZE).good();
    }

    bool dos_header::serialize(std::ostream& stream)
    {
        return stream.write(reinterpret_cast<const char*>(m_data), DOS_HEADER_SIZE).good();
    }

    bool dos_header::is_valid() const
    {
        return reinterpret_cast<const uint16_t*>(m_data)[0] == k_magic;
    }
}