#ifndef PISTON_CORE_FLAG_H
#define PISTON_CORE_FLAG_H

#include <piston/core/buffer.h>

namespace piston
{
    template<typename T>
    class flag : buffer_ref<T>
    {
    public:

        /**
         * @brief Construct a new flag object
         * 
         * @param ptr Pointer to the buffer
         * @param offset Offset into the buffer where the data can be found
         * @param match Value against which to match the buffer value
         */
        flag(buffer_ref<T>::buffer_ptr* ptr, size_t offset, T match) :
        buffer_ref<T>(ptr, offset),
        m_match(match)
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
            return is_set();
        }

        /**
         * @brief Determine whether the value contained in the buffer matches
         * the one provided to the constructor, indicating the flag is set or not.
         * 
         * @return true If the buffer value matches the flag value
         * @return false Otherwise
         */
        bool is_set() const
        {
            return get() == m_match;
        }

    private:

        // Value against which to match the buffer data
        T m_match;
    };
}

#endif // PISTON_CORE_FLAG_H