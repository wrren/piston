#ifndef PISTON_CORE_BUFFER_H
#define PISTON_CORE_BUFFER_H

#include <piston/core/types.h>

namespace Piston
{
    template<typename T>
    class BufferRef
    {
    public:

        typedef uint8_t*    BufferPointer;
        typedef size_t      SizeType;

        /**
         * @brief Construct a new buffer ref object
         * 
         * @param ptr Pointer to the buffer containing the buffer_ref's data
         * @param offset Offset into the buffer where the data begins
         */
        BufferRef(BufferPointer ptr, SizeType offset) :
        mPointer(ptr),
        mOffset(offset)
        {}

        /**
         * @brief Set the value of the underlying buffer data at this ref's offset
         * 
         * @param val New value
         * @return buffer_ref<T>& This object
         */
        BufferRef<T>& operator=(const T& val)
        {
            *(reinterpret_cast<T*>(mPointer + mOffset)) = val;
            return *this;
        }

        /**
         * @brief Cast operator. Returns the value at the ref's offset into the underlying buffer
         * 
         * @return T Value
         */
        operator T() const
        {
            return *(reinterpret_cast<const T*>(mPointer + mOffset));
        }

        /**
         * @brief Get the value pointed at by this buffer reference.
         * 
         * @return T 
         */
        T GetValue() const
        {
            return *(reinterpret_cast<const T*>(mPointer + mOffset));
        }

    private:

        // Pointer to the buffer read and written to by this buffer_ref
        BufferPointer mPointer;
        // Offset into the buffer where this buffer_ref's data begins
        size_t mOffset;
    };
}

#endif // PISTON_CORE_BUFFER_H