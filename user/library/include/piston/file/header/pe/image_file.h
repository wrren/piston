#ifndef PISTON_FILE_HEADER_PE_IMAGE_FILE_H
#define PISTON_FILE_HEADER_PE_IMAGE_FILE_H

#include <piston/file/header.h>

#define PISTON_PE_IMAGE_FILE_HEADER_SIZE 20

namespace Piston
{
    class ImageFileHeader : public Header
    {
    public:

        enum class MachineType : uint16_t
        {
            MACHINE_UNKNOWN = 0,
            MACHINE_AM33    = 0x1d3,
            MACHINE_AMD64   = 0x8664,
            MACHINE_ARM     = 0x1c0,
            MACHINE_ARM64   = 0xaa64,
            MACHINE_I386    = 0x14c
        };

        // Machine Type
        BufferRef<MachineType> Machine;
        // Number of PE sections in the file
        BufferRef<uint16_t> NumberOfSections;
        // Time at which the file was compiled
        BufferRef<uint32_t> Timestamp;
        // Size of the optional header
        BufferRef<uint16_t> OptionalHeaderSize;

        /**
         * @brief Construct a new image file header object
         * 
         */
        ImageFileHeader();

        /**
         * @brief Get the size, in bytes, of this header
         * 
         * @return size_t Header size in bytes
         */
        virtual size_t Size() const override;

        /**
         * @brief Get a pointer to the start of this header's data
         * 
         * @return PointerType Pointer to header data
         */
        virtual PointerType Data() override;
        virtual ConstPointerType Data() const override;

        /**
         * @brief Destroy the image file header object
         * 
         */
        ~ImageFileHeader();

    private:

        // Header data
        byte* mData;
    };
}

#endif // PISTON_FILE_HEADER_PE_IMAGE_FILE_H