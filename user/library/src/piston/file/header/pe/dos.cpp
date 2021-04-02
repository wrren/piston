#include <piston/file/header/pe/dos.h>
#include <cstdlib>

namespace piston
{
    const uint16_t dos_header::k_magic = 0x5A4D;

    dos_header::dos_header() :
    e_magic(m_data, 0),
    e_ifanew(m_data, 60)
    {
        memset(m_data, 0, PISTON_PE_DOS_HEADER_SIZE);
    }

    bool dos_header::deserialize(std::istream& stream)
    {
        return stream.read(reinterpret_cast<char*>(m_data), PISTON_PE_DOS_HEADER_SIZE).good();
    }

    bool dos_header::serialize(std::ostream& stream) const
    {
        return stream.write(reinterpret_cast<const char*>(m_data), PISTON_PE_DOS_HEADER_SIZE).good();
    }

    bool dos_header::is_valid() const
    {
        return e_magic == k_magic;
    }

    size_t dos_header::size() const 
    {
        return PISTON_PE_DOS_HEADER_SIZE;
    }

    header::ptr_type dos_header::data() 
    {
        return &(m_data[0]);
    }

    header::const_ptr_type dos_header::data() const 
    {
        return &(m_data[0]);
    }

}