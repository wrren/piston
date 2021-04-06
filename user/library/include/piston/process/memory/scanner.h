#ifndef PISTON_PROCESS_MEMORY_SCANNER_H
#define PISTON_PROCESS_MEMORY_SCANNER_H

#include <piston/process/memory/region.h>
#include <iterator>

namespace Piston
{
    class MemoryScanner
    {
    public:

        // Scan result type
        typedef std::vector<uintptr_t> ResultType;
        // Pointer to a scanner
        typedef std::shared_ptr<MemoryScanner> PointerType;

        /**
         * @brief Scan the given memory region for matches
         * 
         * @param data Start of data buffer
         * @param data_size Size of buffer in bytes
         * @return result_type Matches
         */
        virtual ResultType Scan(const byte* data, size_t data_size) const = 0;

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
        ResultType ScanForBuffer(const byte* data, size_t data_size, const void* buffer, size_t buffer_size) const;
    };

    template<typename T, typename std::enable_if<std::is_pod<T>::value, T>::type = true>
    class MemoryValueScanner : public MemoryScanner
    {
    public:

        /**
         * @brief Construct a new memory value scanner object
         * 
         * @param value Value to scan for
         */
        MemoryValueScanner(const T& value) :
        mValue(value)
        {}

        /**
         * @brief Scan for a value
         * 
         * @param region Region to be scanned
         * @return result_type Matching regions
         */
        virtual ResultType Scan(const byte* data, size_t data_size) const override
        {
            return ScanForBuffer(data, data_size, reinterpret_cast<const void*>(&mValue), sizeof(T));
        }

    private:

        // The value to scan for
        T mValue;
    };

    template<typename T, typename std::enable_if<std::is_integral<T>::value, T>::type = true>
    class MemoryIntegralScanner : public MemoryScanner
    {
    public:

        enum class CompareMode
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
        MemoryIntegralScanner(const T& value, CompareMode mode = CompareMode::COMPARE_EQUALS) :
        mValue(value),
        mMode(mode)
        {}

        /**
         * @brief Scan for a value based on the configured comparison mode
         * 
         * @param region Region to be scanned
         * @return result_type Matching regions
         */
        virtual ResultType Scan(const byte* data, size_t data_size) const override
        {
            MemoryScanner::ResultType results;

            for(auto i = 0; i < data_size; i++)
            {
                if((i + sizeof(T)) <= data_size)
                {
                    T value = *(reinterpret_cast<const T*>(data + i));

                    if( (mMode == CompareMode::COMPARE_EQUALS                 && value == mValue)    ||
                        (mMode == CompareMode::COMPARE_GREATER_THAN           && value > mValue)     ||
                        (mMode == CompareMode::COMPARE_LESS_THAN              && value < mValue)     ||
                        (mMode == CompareMode::COMPARE_GREATER_THAN_OR_EQUAL  && value >= mValue)    ||
                        (mMode == CompareMode::COMPARE_LESS_THAN_OR_EQUAL     && value <= mValue))
                    {
                        results.push_back(i);
                    }
                }
            }

            return results;
        }

    private:

        // The value to scan for
        T mValue;
        // Comparison Mode
        CompareMode mMode;
    };
}

#endif // PISTON_PROCESS_MEMORY_SCANNER_H