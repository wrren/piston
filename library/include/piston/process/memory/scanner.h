#ifndef PISTON_PROCESS_MEMORY_SCANNER_H
#define PISTON_PROCESS_MEMORY_SCANNER_H

#include <piston/process/memory/region.h>

namespace piston
{
    class memory_scanner
    {
    public:

        // Scan result type
        typedef std::vector<memory_region> result_type;
        // Pointer to a scanner
        typedef std::shared_ptr<memory_scanner> ptr_type;

        /**
         * @brief Scan the given memory region for matches
         * 
         * @param region Region to be scanned
         * @return result_type Matches
         */
        virtual result_type scan(const memory_region& region) const = 0;

    protected:

        /**
         * @brief Scan for the contents of the given memory buffer in the given region
         * 
         * @param region Memory region to be scanned
         * @param buffer Buffer containing data to be scanned for
         * @param buffer_size Size of the buffer
         * @return result_type Matches
         */
        result_type scan_for_buffer(const memory_region& region, void* buffer, size_t buffer_size) const;
    };

    template<typename T, typename std::enable_if<std::is_pod<T>::value, T>::type>
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
        virtual result_type scan(const memory_region& region) const override
        {
            return scan_for_buffer(region, reinterpret_cast<void*>(&m_data), sizeof(T));
        }

    private:

        // The value to scan for
        T m_value;
    };
}

#endif // PISTON_PROCESS_MEMORY_SCANNER_H