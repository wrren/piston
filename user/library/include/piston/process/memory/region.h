#ifndef PISTON_PROCESS_MEMORY_REGION_H
#define PISTON_PROCESS_MEMORY_REGION_H

#include <cstdint>
#include <vector>
#include <piston/core/types.h>

namespace piston
{
    class memory_region
    {
    public:

        // Memory Address Type
        typedef uintptr_t address_type;
        // Region Size Type
        typedef uint64_t size_type;
        // Region list
        typedef std::vector<memory_region> list_type;
        // The difference between two memory regions can be expressed as a set of regions
        typedef list_type diff_type;
        // Memory Data Container
        typedef std::vector<byte> data_type;

        /**
         * @brief Construct a new memory region object with the given start and end addresses
         * 
         * @param start_address Region start address
         * @param end_address Region end address
         */
        memory_region(address_type start_address, address_type end_address);

        /**
         * @brief Get the size of this memory region in bytes
         * 
         * @return data_type::size_type Region size
         */
        data_type::size_type get_size() const;

        /**
         * @brief Get the data contained in this memory region
         * 
         * @return const data_type& Region data
         */
        const data_type& get_data() const;

        /**
         * @brief Get the start address of this memory region.
         * 
         * @return address_type 
         */
        address_type get_start_address() const;

        /**
         * @brief Get the end address of this memory region
         * 
         * @return address_type 
         */
        address_type get_end_address() const;

        /**
         * @brief Determine whether this memory region overlaps with the one specified
         * 
         * @param other Other memory region
         * @return true If this memory region overlaps with the one specified
         * @return false Otherwise
         */
        bool overlaps_with(const memory_region& other) const;

    private:

        // Region Start Address
        address_type m_start_address;
        // Region End Address
        address_type m_end_address;
    };
}

#endif // PISTON_PROCESS_MEMORY_REGION_H