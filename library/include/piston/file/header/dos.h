#ifndef PISTON_FILE_DOS_HEADER_H
#define PISTON_FILE_DOS_HEADER_H

#include <piston/core/core.h>
#include <piston/file/header/header.h>

#define DOS_HEADER_SIZE 64

namespace piston
{
    class dos_header : public header
    {
    public:

        // DOS MZ Magic Number
        static const uint16_t e_magic;

        /**
         * @brief Construct a new DOS header
         * 
         */
        dos_header();

        /**
         * @brief Deserialize a DOS header from the given stream
         * 
         * @param stream Stream from which to deserialize the header data
         * @return true If the data deserialized correctly
         * @return false Otherwise
         */
        virtual bool deserialize(std::istream& stream) override;

        /**
         * @brief Serialize a DOS header to the given stream
         * 
         * @param stream Stream into which to serialize the header data
         * @return true If the data serialized correctly
         * @return false Otherwise
         */
        virtual bool serialize(std::ostream& stream) override;

        /**
         * @brief Determine whether the contained DOS header data is valid
         * 
         * @return true If this DOS header is valid
         * @return false Otherwise
         */
        virtual bool is_valid() const override;

    private:

        // Header Data
        uint8_t m_data[DOS_HEADER_SIZE];
    };
}

#endif // PISTON_FILE_DOS_HEADER_H