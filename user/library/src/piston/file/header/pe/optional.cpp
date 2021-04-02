#include <piston/file/header/pe/optional.h>

namespace piston
{
    image_optional_header::image_optional_header() :
    major_linker_version(&m_data, 2),
    minor_linker_version(&m_data, 3),
    size_of_code(&m_data, 4),
    size_of_initialized_data(&m_data, 8),
    size_of_uninitialized_data(&m_data, 12),
    address_of_entry_point(&m_data, 16),
    base_of_code(&m_data, 20),
    base_of_data(&m_data, 24),
    image_base(&m_data, 28),
    section_alignment(&m_data, 32),
    file_alignment(&m_data, 36),
    major_os_version(&m_data, 40),
    minor_os_version(&m_data, 42),
    major_image_version(&m_data, 44),
    minor_image_version(&m_data, 46),
    is_pe32_plus(&m_data, 0, 0x20B),
    is_rom_image(&m_data, 0, 0x107),
    is_normal_image(&m_data, 0, 0x10B),
    m_size(0),
    m_data(nullptr)
    {}

    image_optional_header& image_optional_header::set_size(size_t size)
    {
        m_size = size;
        if(m_data != nullptr)
        {
            delete[] m_data;
        }
        m_data = new byte[size];

        return *this;
    }

    size_t image_optional_header::size() const 
    {
        return m_size;
    }

    header::ptr_type image_optional_header::data() 
    {
        if(m_data != nullptr)
        {
            return &(m_data[0]);
        }
        return nullptr;
    }

    header::const_ptr_type image_optional_header::data() const 
    {
        if(m_data != nullptr)
        {
            return &(m_data[0]);
        }
        return nullptr;
    }

    image_optional_header::~image_optional_header()
    {
        if(m_data != nullptr)
        {
            delete[] m_data;
        }
    }
}