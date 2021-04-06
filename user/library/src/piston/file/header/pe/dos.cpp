#include <piston/file/header/pe/dos.h>
#include <cstdlib>

namespace Piston
{
    const uint16_t DOSHeader::k_magic = 0x5A4D;

    DOSHeader::DOSHeader() :
    e_magic(m_data, 0),
    e_ifanew(m_data, 60)
    {
        memset(m_data, 0, DOS_HEADER_SIZE);
    }

    bool DOSHeader::Deserialize(std::istream& stream)
    {
        return stream.read(reinterpret_cast<char*>(m_data), DOS_HEADER_SIZE).good();
    }

    bool DOSHeader::Serialize(std::ostream& stream) const
    {
        return stream.write(reinterpret_cast<const char*>(m_data), DOS_HEADER_SIZE).good();
    }

    bool DOSHeader::IsValid() const
    {
        return e_magic == k_magic;
    }

    size_t DOSHeader::Size() const 
    {
        return DOS_HEADER_SIZE;
    }

    Header::PointerType DOSHeader::Data() 
    {
        return &(m_data[0]);
    }

    Header::ConstPointerType DOSHeader::Data() const 
    {
        return &(m_data[0]);
    }

}