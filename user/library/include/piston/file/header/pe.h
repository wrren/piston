#ifndef PISTON_FILE_HEADER_PE_H
#define PISTON_FILE_HEADER_PE_H

#include <piston/file/header.h>

namespace Piston
{
    class PEHeader : public Header
    {
    public:

        /**
         * @brief Deserialize a PE header from the given stream
         * 
         * @param InputStream Stream from which to deserialize the header data
         * @return true If the data deserialized correctly
         * @return false Otherwise
         */
        virtual bool Deserialize(Stream& InputStream) override;
    };
}

#endif // PISTON_FILE_HEADER_PE_H