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
         * @brief Region data iterator type.
         * 
         */
        class iterator
        {
        public:

            /**
             * @brief Construct a new iterator object
             * 
             */
            iterator() = default;

            /**
             * @brief Construct a new iterator object
             * 
             * @param address Address to which the iterator is pointing
             * @param it Data type iterator
             */
            iterator(address_type address, data_type::const_iterator it);

            /**
             * @brief Advance this iterator.
             * 
             * @return iterator& 
             */
            iterator& operator++();

            /**
             * @brief Generate a new iterator by moving the current iterator forward by the specified number of bytes
             * 
             * @param advance Number of bytes by which to advance
             * @return iterator 
             */
            iterator operator+(size_t advance) const;

            /**
             * @brief Move this current iterator forward by the specified number of bytes
             * 
             * @param advance Number of bytes by which to advance
             * @return iterator& 
             */
            iterator& operator+=(size_t advance);

            /**
             * @brief Generate a new iterator by moving the current iterator backwards by the specified number of bytes
             * 
             * @param retreat Number of bytes by which to retreat
             * @return iterator 
             */
            iterator operator-(size_t retreat) const;

            /**
             * @brief Move the current iterator backwards by the specified number of bytes
             * 
             * @param retreat Number of bytes by which to retreat
             * @return iterator& 
             */
            iterator& operator-=(size_t retreat);

            /**
             * @brief Get the data that this iterator is pointing to.
             * 
             * @return byte Data
             */
            byte operator*() const;

            /**
             * @brief Get the pointer to the data being pointed to by this iterator
             * 
             * @return const byte* 
             */
            const byte* get_data() const;

            /**
             * @brief Get the address that this iterator is currently pointing to.
             * 
             * @return address_type Iterator address
             */
            address_type get_address() const;

            /**
             * @brief Get the data iterator
             * 
             * @return data_type::const_iterator Data iterator
             */
            data_type::const_iterator get_data_iterator() const;

            /**
             * @brief Comparison operators
             */
            bool operator==(const iterator& other) const;
            bool operator!=(const iterator& other) const;
            bool operator<=(const iterator& other) const;
            bool operator>=(const iterator& other) const;
            bool operator<(const iterator& other) const;
            bool operator>(const iterator& other) const;

        private:

            // Current Address
            address_type m_address;
            // Data vector iterator
            data_type::const_iterator m_iterator;
        };

        /**
         * @brief Construct a new memory region object with the given base address and data
         * 
         * @param base_address Region base address
         * @param data Data
         */
        memory_region(address_type base_address, const data_type& data = {});

        /**
         * @brief Construct a new memory region object with the given base address and data from the given iterators
         * 
         * @param base_address Region base address
         * @param begin Beginning of data
         * @param end End of data
         */
        memory_region(address_type base_address, data_type::const_iterator begin, data_type::const_iterator end);

        /**
         * @brief Construct a new memory region object with the given base address and data from the given iterators
         * 
         * @param begin Beginning of data
         * @param end End of data
         */
        memory_region(iterator begin, iterator end);

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
         * @brief Get the base address of this memory region.
         * 
         * @return const address_type 
         */
        address_type get_base_address() const;

        /**
         * @brief Get the difference between this region and the one provided. The two regions must have some overlap
         * in order for a diff to be generated. The diff will be generated by the difference between their overlapping
         * regions and returned as a set of memory regions.
         * 
         * @param other Memory region to diff against
         * @return diff_type Differences between regions
         */
        diff_type diff(const memory_region& other) const;

        /**
         * @brief Determine whether this memory region overlaps with the one specified
         * 
         * @param other Other memory region
         * @return true If this memory region overlaps with the one specified
         * @return false Otherwise
         */
        bool overlaps_with(const memory_region& other) const;

        /**
         * @brief Get an iterator to the beginning of this memory region
         * 
         * @return iterator Memory region iterator
         */
        iterator begin() const;

        /**
         * @brief Get an iterator to the data at the given offset within this region
         * 
         * @param offset Offset
         * @return iterator Memory region iterator
         */
        iterator offset(const size_type offset) const;

        /**
         * @brief Get an iterator to the end of this memory region
         * 
         * @return iterator Memory region iterator
         */
        iterator end() const;

    private:

        // Region Base Address
        address_type m_base_address;
        // Contained Data
        data_type m_data;
    };
}

#endif // PISTON_PROCESS_MEMORY_REGION_H