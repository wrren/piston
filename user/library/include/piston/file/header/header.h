#ifndef PISTON_FILE_HEADER_H
#define PISTON_FILE_HEADER_H

#include <piston/core/serializable.h>

namespace piston
{
    class header : public serializable
    {
    public:

        // Pointer to data in the header
        typedef uint8_t*        ptr_type;
        typedef const uint8_t*  const_ptr_type;

        /**
         * @brief Check whether this header's data is valid.
         * 
         * @return true If the header data is valid
         * @return false Otherwise
         */
        virtual bool is_valid() const;

        /**
         * @brief Get the size, in bytes, of this header
         * 
         * @return size_t Header size in bytes
         */
        virtual size_t size() const = 0;

        /**
         * @brief Get a pointer to the start of this header's data
         * 
         * @return ptr_type Pointer to header data
         */
        virtual ptr_type data() = 0;
        virtual const_ptr_type data() const = 0;
    };
}

#endif // PISTON_FILE_HEADER_H