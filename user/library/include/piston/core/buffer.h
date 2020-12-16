#ifndef PISTON_CORE_BUFFER_H
#define PISTON_CORE_BUFFER_H

namespace piston
{
    template<typename T>
    class buffer_ref
    {
    public:

        typedef uint8_t* buffer_ptr;

        /**
         * @brief Construct a new buffer ref object
         * 
         * @param ptr Pointer to the buffer containing the buffer_ref's data
         * @param offset Offset into the buffer where the data begins
         */
        buffer_ref(buffer_ptr ptr, size_t offset) :
        m_ptr(ptr),
        m_offset(offset)
        {}

        /**
         * @brief Set the value of the underlying buffer data at this ref's offset
         * 
         * @param val New value
         * @return buffer_ref<T>& This object
         */
        buffer_ref<T>& operator=(const T& val)
        {
            *(reinterpret_cast<T*>(m_ptr + m_offset)) = val;
            return *this;
        }

        /**
         * @brief Cast operator. Returns the value at the ref's offset into the underlying buffer
         * 
         * @return T Value
         */
        operator T() const
        {
            return *(reinterpret_cast<const T*>(m_ptr + m_offset));
        }

    private:

        // Pointer to the buffer read and written to by this buffer_ref
        buffer_ptr m_ptr;
        // Offset into the buffer where this buffer_ref's data begins
        size_t m_offset;
    };
}

#endif // PISTON_CORE_BUFFER_H