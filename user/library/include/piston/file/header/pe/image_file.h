#ifndef PISTON_FILE_HEADER_PE_IMAGE_FILE_H
#define PISTON_FILE_HEADER_PE_IMAGE_FILE_H

#include <piston/file/header.h>

namespace piston
{
    class image_file_header : public header
    {
    public:

        // Header size in bytes
        static const size_t header_size;

        /**
         * @brief Deserialize an image file header from the given stream
         * 
         * @param stream Stream from which to deserialize the header data
         * @return true If the data deserialized correctly
         * @return false Otherwise
         */
        virtual bool deserialize(std::istream& stream) override;
    };
}

#endif // PISTON_FILE_HEADER_PE_IMAGE_FILE_H