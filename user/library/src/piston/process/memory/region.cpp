#include <piston/process/memory/region.h>
#include <iterator>

namespace Piston
{
    MemoryRegion::MemoryRegion(MemoryRegion::AddressType start_address, MemoryRegion::SizeType size) :
    mStartAddress(start_address),
    mSize(size)
    {}

    MemoryRegion::SizeType MemoryRegion::GetSize() const
    {
        return mSize;
    }

    MemoryRegion::AddressType MemoryRegion::GetStartAddress() const
    {
        return mStartAddress;
    }

    MemoryRegion::AddressType MemoryRegion::GetEndAddress() const
    {
        return mStartAddress + mSize;
    }

    bool MemoryRegion::OverlapsWith(const MemoryRegion& other) const
    {
        return  (mStartAddress <= other.mStartAddress && (mStartAddress + mSize) >= other.mStartAddress) ||
                (mStartAddress >= other.mStartAddress && mStartAddress <= (other.mStartAddress + other.mSize));
    }

    bool MemoryRegion::Deserialize(Stream& InputStream)
    {
        InputStream >> mStartAddress >> mSize;
        return InputStream.Good();
    }

    bool MemoryRegion::Serialize(Stream& OutputStream) const
    {
        OutputStream << mStartAddress << mSize;
        return OutputStream.Good();
    }
}