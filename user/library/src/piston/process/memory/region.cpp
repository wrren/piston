#include <piston/process/memory/region.h>
#include <iterator>

namespace piston
{
    memory_region::memory_region(memory_region::address_type start_address, memory_region::address_type end_address) :
    m_start_address(start_address),
    m_end_address(end_address)
    {}

    memory_region::data_type::size_type memory_region::get_size() const
    {
        return m_end_address - m_start_address;
    }

    memory_region::address_type memory_region::get_start_address() const
    {
        return m_start_address;
    }

    memory_region::address_type memory_region::get_end_address() const
    {
        return m_end_address;
    }

    bool memory_region::overlaps_with(const memory_region& other) const
    {
        return  (m_start_address <= other.m_start_address && m_end_address >= other.m_start_address) ||
                (m_start_address >= other.m_start_address && m_start_address <= other.m_end_address);
    }
}