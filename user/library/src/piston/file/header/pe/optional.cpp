#include <piston/file/header/pe/optional.h>

namespace Piston
{
    ImageOptionalHeader::ImageOptionalHeader() :
    MajorLinkerVersion(mData, 2),
    MinorLinkerVersion(mData, 3),
    SizeOfCode(mData, 4),
    SizeOfInitializedData(mData, 8),
    SizeOfUninitializedData(mData, 12),
    AddressOfEntryPoint(mData, 16),
    BaseOfCode(mData, 20),
    BaseOfData(mData, 24),
    ImageBase(mData, 28),
    SectionAlignment(mData, 32),
    FileAlignment(mData, 36),
    MajorOSVersion(mData, 40),
    MinorOSVersion(mData, 42),
    MajorImageVersion(mData, 44),
    MinorImageVersion(mData, 46),
    IsPE32Plus(mData, 0, 0x20B),
    IsROMImage(mData, 0, 0x107),
    IsNormalImage(mData, 0, 0x10B),
    mSize(0),
    mData(nullptr)
    {}

    ImageOptionalHeader& ImageOptionalHeader::SetSize(size_t size)
    {
        mSize = size;
        if(mData != nullptr)
        {
            delete[] mData;
        }
        mData = new byte[size];

        return *this;
    }

    size_t ImageOptionalHeader::Size() const 
    {
        return mSize;
    }

    Header::PointerType ImageOptionalHeader::Data() 
    {
        if(mData != nullptr)
        {
            return &(mData[0]);
        }
        return nullptr;
    }

    Header::ConstPointerType ImageOptionalHeader::Data() const 
    {
        if(mData != nullptr)
        {
            return &(mData[0]);
        }
        return nullptr;
    }

    ImageOptionalHeader::~ImageOptionalHeader()
    {
        if(mData != nullptr)
        {
            delete[] mData;
        }
    }
}