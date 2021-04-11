#ifndef PISTON_PROCESS_MEMORY_REGION_H
#define PISTON_PROCESS_MEMORY_REGION_H

#include <cstdint>
#include <vector>
#include <piston/core/types.h>
#include <piston/core/serializable.h>

namespace Piston
{
    class MemoryRegion : public Serializable
    {
    public:

        // Memory Address Type
        typedef uintptr_t AddressType;
        // Region Size Type
        typedef uint64_t SizeType;
        // Region list
        typedef std::vector<MemoryRegion> ListType;

        /**
         * @brief Construct a new Memory Region object
         * 
         */
        MemoryRegion() = default;

        /**
         * @brief Construct a new memory region object with the given start and end addresses
         * 
         * @param start_address Region start address
         * @param size Region size
         */
        MemoryRegion(AddressType start_address, SizeType size);

        /**
         * @brief Get the size of this memory region in bytes
         * 
         * @return data_type::size_type Region size
         */
        SizeType GetSize() const;

        /**
         * @brief Get the start address of this memory region.
         * 
         * @return address_type 
         */
        AddressType GetStartAddress() const;

        /**
         * @brief Get the end address of this memory region
         * 
         * @return address_type 
         */
        AddressType GetEndAddress() const;

        /**
         * @brief Determine whether this memory region overlaps with the one specified
         * 
         * @param other Other memory region
         * @return true If this memory region overlaps with the one specified
         * @return false Otherwise
         */
        bool OverlapsWith(const MemoryRegion& other) const;

        /**
         * @brief Deserialize data from the given stream
         * 
         * @param InputStream Input stream
         * @return true If this object was deserialized correctly
         * @return false Otherwise
         */
        virtual bool Deserialize(Stream& InputStream) override;

        /**
         * @brief Serialize data into the given stream
         * 
         * @param OutputStream Output stream
         * @return true If this object was serialized correctly
         * @return false Otherwise
         */
        virtual bool Serialize(Stream& OutputStream) const override;

    private:

        // Region Start Address
        AddressType mStartAddress;
        // Region size
        SizeType mSize;
    };
}

#endif // PISTON_PROCESS_MEMORY_REGION_H