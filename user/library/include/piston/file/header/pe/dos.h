#ifndef PISTON_FILE_DOS_HEADER_H
#define PISTON_FILE_DOS_HEADER_H

#include <piston/core/core.h>
#include <piston/file/header.h>

#define DOS_HEADER_SIZE 64

namespace Piston
{
    class DOSHeader : public Header
    {
    public:

        // DOS MZ Magic Number
        static const uint16_t kMagic;

        // Magic Value
        BufferRef<uint16_t> eMagic;
        // Offset to PE header
        BufferRef<uint32_t> eIFANew;

        /**
         * @brief Construct a new DOS header
         * 
         */
        DOSHeader();

        /**
         * @brief Deserialize a DOS header from the given stream
         * 
         * @param InputStream Stream from which to deserialize the header data
         * @return true If the data deserialized correctly
         * @return false Otherwise
         */
        virtual bool Deserialize(Stream& InputStream) override;

        /**
         * @brief Serialize a DOS header to the given stream
         * 
         * @param OutputStream Stream into which to serialize the header data
         * @return true If the data serialized correctly
         * @return false Otherwise
         */
        virtual bool Serialize(Stream& OutputStream) const override;

        /**
         * @brief Determine whether the contained DOS header data is valid
         * 
         * @return true If this DOS header is valid
         * @return false Otherwise
         */
        virtual bool IsValid() const override;

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

    private:

        // Header Data
        uint8_t mData[DOS_HEADER_SIZE];
    };
}

#endif // PISTON_FILE_DOS_HEADER_H