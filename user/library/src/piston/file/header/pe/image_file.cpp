#include <piston/file/header/pe/image_file.h>

namespace piston
{
    image_file_header::image_file_header() :
    machine(&m_data, 0),
    number_of_sections(&m_data, 2),
    timestamp(&m_data, 4),
    optional_header_size(&m_data, 16)
    {
        m_data = new byte[PISTON_PE_IMAGE_FILE_HEADER_SIZE];
    }

    size_t image_file_header::size() const 
    {
        return PISTON_PE_IMAGE_FILE_HEADER_SIZE;
    }

    header::ptr_type image_file_header::data() 
    {
        return m_data;
    }

    header::const_ptr_type image_file_header::data() const 
    {
        return m_data;
    }

    image_file_header::~image_file_header()
    {
        delete[] m_data;
    }
}