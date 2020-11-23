#ifndef PISTON_FILE_HEADER_H
#define PISTON_FILE_HEADER_H

#include <piston/core/serializable.h>

namespace piston
{
    class header : public serializable
    {
    public:

        /**
         * @brief Check whether this header's data is valid.
         * 
         * @return true If the header data is valid
         * @return false Otherwise
         */
        virtual bool is_valid() const;
    };
}

#endif // PISTON_FILE_HEADER_H