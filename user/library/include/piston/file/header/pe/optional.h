#ifndef PISTON_FILE_HEADER_PE_OPTIONAL_H
#define PISTON_FILE_HEADER_PE_OPTIONAL_H

#include <piston/file/header.h>

#define PISTON_PE_DATA_DIRECTORY_COUNT 16

namespace piston
{
    class image_optional_header : public header
    {
    public:

        struct data_directory
        {
            uint32_t virtual_address;
            uint32_t size;
        };

        buffer_ref<byte> major_linker_version;
        buffer_ref<byte> minor_linker_version;
        buffer_ref<uint32_t> size_of_code;
        buffer_ref<uint32_t> size_of_initialized_data;
        buffer_ref<uint32_t> size_of_uninitialized_data;
        buffer_ref<uint32_t> address_of_entry_point;
        buffer_ref<uint32_t> base_of_code;
        buffer_ref<uint32_t> base_of_data;
        buffer_ref<uint32_t> image_base;
        buffer_ref<uint32_t> section_alignment;
        buffer_ref<uint32_t> file_alignment;
        buffer_ref<uint32_t> major_os_version;
        buffer_ref<uint32_t> minor_os_version;
        buffer_ref<uint32_t> major_image_version;
        buffer_ref<uint32_t> minor_image_version;

        // Indicates whether this is a PE32+ executable
        flag<uint16_t> is_pe32_plus;
        flag<uint16_t> is_rom_image;
        flag<uint16_t> is_normal_image;

        /**
         * @brief Construct a new image optional header object
         * 
         */
        image_optional_header();

        /**
         * @brief Set the size of the optional header
         * 
         * @param size 
         * @return image_optional_header& 
         */
        image_optional_header& set_size(size_t size);

        /**
         * @brief Get the size, in bytes, of this header
         * 
         * @return size_t Header size in bytes
         */
        virtual size_t size() const override;

        /**
         * @brief Get a pointer to the start of this header's data
         * 
         * @return ptr_type Pointer to header data
         */
        virtual ptr_type data() override;
        virtual const_ptr_type data() const override;

        /**
         * @brief Destroy the image optional header object
         * 
         */
        ~image_optional_header();

    private:

        // Header data
        byte* m_data;
        // Size of the header
        size_t m_size;
    };
}

#endif // PISTON_FILE_HEADER_PE_OPTIONAL_H