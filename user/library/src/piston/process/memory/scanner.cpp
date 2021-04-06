#include <piston/process/memory/scanner.h>

namespace Piston
{
    MemoryScanner::ResultType MemoryScanner::ScanForBuffer(const byte* data, size_t data_size, const void* buffer, size_t buffer_size) const    
    {
        MemoryScanner::ResultType results;

        if(buffer_size == 0)
        {
            return results;
        }

        size_t offset = 0;
        
        for(auto i = 0; i < data_size; i++)
        {
            if((i + buffer_size) >= data_size)
            {
                break;
            }

            if(memcmp(data + i, buffer, buffer_size) == 0)
            {
                results.push_back(i);
            }
        }

        return results;
    }
}