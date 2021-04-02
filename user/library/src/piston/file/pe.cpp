#include <piston/file/pe.h>

#define PISTON_PE_NT_SIGNATURE 0x00004550

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

    const dos_header& pe_file::get_dos_header() const 
    {
        return m_dos_header;
    }

    const image_file_header& pe_file::get_image_file_header() const
    {
        return m_image_file_header;
    }

    const image_optional_header& pe_file::get_image_optional_header() const
    {
        return m_image_optional_header;
    }

    bool pe_file::deserialize(std::istream& stream)
    {
        if(m_dos_header.deserialize(stream))
        {
            stream.seekg(m_dos_header.e_ifanew.get());
            uint32_t nt_signature;
            stream.read(reinterpret_cast<char*>(&nt_signature), sizeof(nt_signature));

            if(nt_signature == PISTON_PE_NT_SIGNATURE && m_image_file_header.deserialize(stream))
            {
                m_image_optional_header.set_size(m_image_file_header.optional_header_size.get());
                return m_image_optional_header.deserialize(stream);
            }            
        }
        return false;
    }
}