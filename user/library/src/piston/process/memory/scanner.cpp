#include <piston/process/memory/scanner.h>

namespace Piston
{
    MemoryBufferScanner::MemoryBufferScanner(const byte* TargetBuffer, size_t TargetBufferSize) :
    mTargetBuffer(TargetBuffer),
    mTargetBufferSize(TargetBufferSize)
    {}

    MemoryScanner::ResultType MemoryBufferScanner::Scan(const byte* Data, size_t DataSize) const
    {
        return ScanForBuffer(Data, DataSize, mTargetBuffer, mTargetBufferSize);
    }

    MemoryScanner::ResultType MemoryScanner::ScanForBuffer(const byte* Data, size_t DataSize, const void* Buffer, size_t BufferSize) const    
    {
        MemoryScanner::ResultType Results;

        if(BufferSize == 0)
        {
            return Results;
        }

        size_t Offset = 0;
        
        for(auto i = 0; i < DataSize; i++)
        {
            if((i + BufferSize) > DataSize)
            {
                break;
            }

            if(memcmp(Data + i, Buffer, BufferSize) == 0)
            {
                Results.push_back(i);
            }
        }

        return Results;
    }
}