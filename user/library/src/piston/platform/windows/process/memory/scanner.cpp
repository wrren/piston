#include <piston/process/memory/scanner.h>

namespace piston
{
    memory_scanner::result_type memory_scanner::scan_for_buffer(const memory_region& region, const void* buffer, size_t buffer_size) const    
    {
        memory_scanner::result_type results;

        if(buffer_size == 0)
        {
            return results;
        }

        size_t offset = 0;
        auto begin = region.begin();

        for(auto it = region.begin(); it != region.end(); ++it)
        {
            if(offset == buffer_size)
            {
                results.push_back(memory_region(begin, it));
                offset = 0;
            } 
            else if(*it == reinterpret_cast<const byte*>(buffer)[offset])
            {
                if(offset == 0)
                {
                    begin = it;
                }
                ++offset;
            }
        }

        return results;
    }
}