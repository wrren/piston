#include <piston/file/header/dos.h>
#include <cstdlib>

namespace piston
{
    const uint16_t dos_header::e_magic = 0x54AD;

    dos_header::dos_header()
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
        return reinterpret_cast<const uint16_t*>(m_data)[0] == e_magic;
    }
}