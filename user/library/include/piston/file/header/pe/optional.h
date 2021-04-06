#ifndef PISTON_FILE_HEADER_PE_OPTIONAL_H
#define PISTON_FILE_HEADER_PE_OPTIONAL_H

#include <piston/file/header.h>

#define PISTON_PE_DATA_DIRECTORY_COUNT 16

namespace Piston
{
    class ImageOptionalHeader : public Header
    {
    public:

        struct DataDirectory
        {
            uint32_t VirtualAddress;
            uint32_t Size;
        };

        BufferRef<byte> MajorLinkerVersion;
        BufferRef<byte> MinorLinkerVersion;
        BufferRef<uint32_t> SizeOfCode;
        BufferRef<uint32_t> SizeOfInitializedData;
        BufferRef<uint32_t> SizeOfUninitializedData;
        BufferRef<uint32_t> AddressOfEntryPoint;
        BufferRef<uint32_t> BaseOfCode;
        BufferRef<uint32_t> BaseOfData;
        BufferRef<uint32_t> ImageBase;
        BufferRef<uint32_t> SectionAlignment;
        BufferRef<uint32_t> FileAlignment;
        BufferRef<uint32_t> MajorOSVersion;
        BufferRef<uint32_t> MinorOSVersion;
        BufferRef<uint32_t> MajorImageVersion;
        BufferRef<uint32_t> MinorImageVersion;

        // Indicates whether this is a PE32+ executable
        Flag<uint16_t> IsPE32Plus;
        Flag<uint16_t> IsROMImage;
        Flag<uint16_t> IsNormalImage;

        /**
         * @brief Construct a new image optional header object
         * 
         */
        ImageOptionalHeader();

        /**
         * @brief Set the size of the optional header
         * 
         * @param size 
         * @return ImageOptionalHeader& 
         */
        ImageOptionalHeader& SetSize(size_t size);

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
         * @brief Destroy the image optional header object
         * 
         */
        ~ImageOptionalHeader();

    private:

        // Header data
        byte* mData;
        // Size of the header
        size_t mSize;
    };
}

#endif // PISTON_FILE_HEADER_PE_OPTIONAL_H