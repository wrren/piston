#ifndef PISTON_PROCESS_MEMORY_SCANNER_H
#define PISTON_PROCESS_MEMORY_SCANNER_H

#include <piston/process/memory/region.h>
#include <iterator>

namespace piston
{
    class memory_scanner
    {
    public:

        // Scan result type
        typedef std::vector<uintptr_t> result_type;
        // Pointer to a scanner
        typedef std::shared_ptr<memory_scanner> ptr_type;

        /**
         * @brief Scan the given memory region for matches
         * 
         * @param data Start of data buffer
         * @param data_size Size of buffer in bytes
         * @return result_type Matches
         */
        virtual result_type scan(const byte* data, size_t data_size) const = 0;

    protected:

        /**
         * @brief Scan for the contents of the given memory buffer in the given region
         * 
         * @param data Start of input data
         * @param data_size Size of input data in bytes
         * @param buffer Buffer containing data to be scanned for
         * @param buffer_size Size of the buffer
         * @return result_type Matches
         */
        result_type scan_for_buffer(const byte* data, size_t data_size, const void* buffer, size_t buffer_size) const;
    };

    template<typename T, typename std::enable_if<std::is_pod<T>::value, T>::type = true>
    class memory_value_scanner : public memory_scanner
    {
    public:

        /**
         * @brief Construct a new memory value scanner object
         * 
         * @param value Value to scan for
         */
        memory_value_scanner(const T& value) :
        m_value(value)
        {}

        /**
         * @brief Scan for a value
         * 
         * @param region Region to be scanned
         * @return result_type Matching regions
         */
        virtual result_type scan(const byte* data, size_t data_size) const override
        {
            return scan_for_buffer(data, data_size, reinterpret_cast<const void*>(&m_value), sizeof(T));
        }

    private:

        // The value to scan for
        T m_value;
    };

    template<typename T, typename std::enable_if<std::is_integral<T>::value, T>::type = true>
    class memory_integral_scanner : public memory_scanner
    {
    public:

        enum class compare_mode
        {
            COMPARE_EQUALS,
            COMPARE_GREATER_THAN,
            COMPARE_LESS_THAN,
            COMPARE_GREATER_THAN_OR_EQUAL,
            COMPARE_LESS_THAN_OR_EQUAL
        };

        /**
         * @brief Construct a new memory value scanner object
         * 
         * @param value Value to scan for
         */
        memory_integral_scanner(const T& value, compare_mode mode = compare_mode::COMPARE_EQUALS) :
        m_value(value),
        m_mode(mode)
        {}

        /**
         * @brief Scan for a value based on the configured comparison mode
         * 
         * @param region Region to be scanned
         * @return result_type Matching regions
         */
        virtual result_type scan(const byte* data, size_t data_size) const override
        {
            memory_scanner::result_type results;

            for(auto i = 0; i < data_size; i++)
            {
                if((i + sizeof(T)) <= data_size)
                {
                    T value = *(reinterpret_cast<const T*>(data + i));

                    if( (m_mode == compare_mode::COMPARE_EQUALS                 && value == m_value)    ||
                        (m_mode == compare_mode::COMPARE_GREATER_THAN           && value > m_value)     ||
                        (m_mode == compare_mode::COMPARE_LESS_THAN              && value < m_value)     ||
                        (m_mode == compare_mode::COMPARE_GREATER_THAN_OR_EQUAL  && value >= m_value)    ||
                        (m_mode == compare_mode::COMPARE_LESS_THAN_OR_EQUAL     && value <= m_value))
                    {
                        results.push_back(i);
                    }
                }
            }

            return results;
        }

    private:

        // The value to scan for
        T m_value;
        // Comparison Mode
        compare_mode m_mode;
    };
}

#endif // PISTON_PROCESS_MEMORY_SCANNER_H