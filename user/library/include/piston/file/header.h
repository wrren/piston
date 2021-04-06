#ifndef PISTON_FILE_HEADER_H
#define PISTON_FILE_HEADER_H

#include <piston/core/core.h>
#include <piston/core/serializable.h>
#include <piston/core/buffer.h>

namespace Piston
{
    class Header : public Serializable
    {
    public:

        // Pointer to data in the header
        typedef uint8_t*        PointerType;
        typedef const uint8_t*  ConstPointerType;

        /**
         * @brief Check whether this header's data is valid.
         * 
         * @return true If the header data is valid
         * @return false Otherwise
         */
        virtual bool IsValid() const;

        /**
         * @brief Get the size, in bytes, of this header
         * 
         * @return size_t Header size in bytes
         */
        virtual size_t Size() const = 0;

        /**
         * @brief Get a pointer to the start of this header's data
         * 
         * @return PointerType Pointer to header data
         */
        virtual PointerType Data() = 0;
        virtual ConstPointerType Data() const = 0;
    };
}

#endif // PISTON_FILE_HEADER_H