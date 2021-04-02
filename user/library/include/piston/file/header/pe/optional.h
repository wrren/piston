#ifndef PISTON_FILE_HEADER_PE_OPTIONAL_H
#define PISTON_FILE_HEADER_PE_OPTIONAL_H

#include <piston/file/header.h>

namespace piston
{
    class optional_header : public header
    {
    public:

        /**
         * @brief Deserialize an image optional header from the given stream
         * 
         * @param stream Stream from which to deserialize the header data
         * @return true If the data deserialized correctly
         * @return false Otherwise
         */
        virtual bool deserialize(std::istream& stream) override;
    };
}

#endif // PISTON_FILE_HEADER_PE_OPTIONAL_H