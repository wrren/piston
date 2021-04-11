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
         * @param Data Start of Data Buffer
         * @param DataSize Size of Buffer in bytes
         * @return result_type Matches
         */
        virtual ResultType Scan(const byte* Data, size_t DataSize) const = 0;

    protected:

        /**
         * @brief Scan for the contents of the given memory Buffer in the given region
         * 
         * @param Data Start of input Data
         * @param DataSize Size of input Data in bytes
         * @param Buffer Buffer containing Data to be scanned for
         * @param BufferSize Size of the Buffer
         * @return result_type Matches
         */
        ResultType ScanForBuffer(const byte* Data, size_t DataSize, const void* Buffer, size_t BufferSize) const;
    };

    class MemoryBufferScanner : public MemoryScanner
    {
    public:

        MemoryBufferScanner(const byte* TargetBuffer, size_t TargetBufferSize);

        /**
         * @brief Scan the given memory region for matches
         * 
         * @param Data Start of Data Buffer
         * @param DataSize Size of Buffer in bytes
         * @return result_type Matches
         */
        virtual ResultType Scan(const byte* Data, size_t DataSize) const override;

    private:

        // Target buffer
        const byte* mTargetBuffer;
        // Target buffer size
        size_t mTargetBufferSize;
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
        virtual ResultType Scan(const byte* Data, size_t DataSize) const override
        {
            return ScanForBuffer(Data, DataSize, reinterpret_cast<const void*>(&mValue), sizeof(T));
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
        virtual ResultType Scan(const byte* Data, size_t DataSize) const override
        {
            MemoryScanner::ResultType results;

            for(auto i = 0; i < DataSize; i++)
            {
                if((i + sizeof(T)) <= DataSize)
                {
                    T value = *(reinterpret_cast<const T*>(Data + i));

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