#include <piston/process/memory/region.h>
#include <iterator>

namespace piston
{
    memory_region::iterator::iterator(memory_region::address_type address, data_type::const_iterator it) :
    m_address(address),
    m_iterator(it)
    {}

    memory_region::iterator& memory_region::iterator::operator++()
    {
        m_iterator++;
        m_address++;
        return *this;
    }
    
    byte memory_region::iterator::operator*() const
    {
        return *m_iterator;
    }

    memory_region::data_type::const_iterator memory_region::iterator::get_data_iterator() const
    {
        return m_iterator;
    }

    memory_region::address_type memory_region::iterator::get_address() const
    {
        return m_address;
    }

    bool memory_region::iterator::operator==(const memory_region::iterator& other) const
    {
        return m_address == other.m_address && m_iterator == other.m_iterator;
    }

    bool memory_region::iterator::operator!=(const memory_region::iterator& other) const
    {
        return !(*this == other);
    }
    
    memory_region::memory_region(memory_region::address_type base_address, const memory_region::data_type& data) :
    m_base_address(base_address),
    m_data(data)
    {}

    memory_region::memory_region(memory_region::address_type base_address, iterator begin, iterator end) :
    m_base_address(base_address),
    m_data(begin.get_data_iterator(), end.get_data_iterator())
    {}
    
    memory_region::memory_region(memory_region::address_type base_address, data_type::const_iterator begin, data_type::const_iterator end) :
    m_base_address(base_address),
    m_data(begin, end)
    {}

    memory_region::data_type::size_type memory_region::get_size() const
    {
        return m_data.size();
    }

    const memory_region::data_type& memory_region::get_data() const
    {
        return m_data;
    }

    memory_region::address_type memory_region::get_base_address() const
    {
        return m_base_address;
    }

    memory_region::diff_type memory_region::diff(const memory_region& other) const
    {
        diff_type diff;

        if(overlaps_with(other))
        {
            iterator m_begin, m_end, o_begin, o_end;   
            if(m_base_address <= other.m_base_address)
            {
                m_begin = offset(other.m_base_address - m_base_address);
                o_begin = other.begin();
            }
            else
            {
                o_begin = other.offset(m_base_address - other.m_base_address);
                m_begin = begin();
            }

            if(m_base_address + m_data.size() >= other.m_base_address + other.m_data.size())
            {
                m_end = offset((other.m_base_address + other.m_data.size()) - m_base_address);
                o_end = other.end();
            }
            else
            {
                m_end = end();
                o_end = other.offset((m_base_address + m_data.size()) - other.m_base_address);
            }

            auto region_start   = o_begin;
            auto region_end     = o_begin;
            auto region_size    = 0;

            for(auto m_it = m_begin, o_it = o_begin; m_it != m_end && o_it != o_end; ++m_it, ++o_it)
            {
                if(*m_it == *o_it && region_size > 0)
                {
                    diff.push_back(memory_region(o_it.get_address(), region_start, o_it));
                    region_start    = o_it;
                    region_size     = 0;
                }
                else if(*m_it != *o_it)
                {
                    region_size++;
                    region_end      = o_it;
                }
            }
            
            if(region_size > 0)
            {
                diff.push_back(memory_region(region_start.get_address(), region_start, ++region_end));
            }
        }

        return diff;
    }

    bool memory_region::overlaps_with(const memory_region& other) const
    {
        return  (m_base_address <= other.m_base_address && m_base_address + m_data.size() >= other.m_base_address) ||
                (m_base_address >= other.m_base_address && m_base_address <= other.m_base_address + other.m_data.size());
    }


    memory_region::iterator memory_region::begin() const
    {
        return memory_region::iterator(m_base_address, m_data.begin());
    }   

    memory_region::iterator memory_region::offset(const size_type offset) const
    {
        if(offset >= m_data.size())
        {
            return end();
        }
        auto it = m_data.begin();
        std::advance(it, offset);
        return memory_region::iterator(m_base_address + offset, it);
    }

    memory_region::iterator memory_region::end() const
    {
        return memory_region::iterator(m_base_address + m_data.size(), m_data.end());
    }
}