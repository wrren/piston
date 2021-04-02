#ifndef PISTON_FILE_HEADER_PE_IMAGE_FILE_H
#define PISTON_FILE_HEADER_PE_IMAGE_FILE_H

#include <piston/file/header.h>

#define PISTON_PE_IMAGE_FILE_HEADER_SIZE 20

namespace piston
{
    class image_file_header : public header
    {
    public:

        enum class machine_type : uint16_t
        {
            MACHINE_UNKNOWN = 0,
            MACHINE_AM33    = 0x1d3,
            MACHINE_AMD64   = 0x8664,
            MACHINE_ARM     = 0x1c0,
            MACHINE_ARM64   = 0xaa64,
            MACHINE_I386    = 0x14c
        };

        // Machine Type
        buffer_ref<machine_type> machine;
        // Number of PE sections in the file
        buffer_ref<uint16_t> number_of_sections;
        // Time at which the file was compiled
        buffer_ref<uint32_t> timestamp;
        // Size of the optional header
        buffer_ref<uint16_t> optional_header_size;

        /**
         * @brief Construct a new image file header object
         * 
         */
        image_file_header();

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
         * @brief Destroy the image file header object
         * 
         */
        ~image_file_header();

    private:

        // Header data
        byte* m_data;
    };
}

#endif // PISTON_FILE_HEADER_PE_IMAGE_FILE_H