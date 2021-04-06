#ifndef PISTON_CORE_FLAG_H
#define PISTON_CORE_FLAG_H

#include <piston/core/buffer.h>

namespace Piston
{
    template<typename T>
    class Flag : BufferRef<T>
    {
    public:

        /**
         * @brief Construct a new flag object
         * 
         * @param ptr Pointer to the buffer
         * @param offset Offset into the buffer where the data can be found
         * @param match Value against which to match the buffer value
         */
        Flag(BufferRef<T>::BufferPointer ptr, SizeType offset, T match) :
        BufferRef<T>(ptr, offset),
        mMatch(match)
        {}

        /**
         * @brief Determine whether the value contained in the buffer matches
         * the one provided to the constructor, indicating the flag is set or not.
         * 
         * @return true If the buffer value matches the flag value
         * @return false Otherwise
         */
        operator bool() const
        {
            return IsSet();
        }

        /**
         * @brief Determine whether the value contained in the buffer matches
         * the one provided to the constructor, indicating the flag is set or not.
         * 
         * @return true If the buffer value matches the flag value
         * @return false Otherwise
         */
        bool IsSet() const
        {
            return GetValue() == mMatch;
        }

    private:

        // Value against which to match the buffer data
        T mMatch;
    };
}

#endif // PISTON_CORE_FLAG_H