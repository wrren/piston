#include <piston/file/header/pe/image_file.h>

namespace Piston
{
    ImageFileHeader::ImageFileHeader() :
    Machine(mData, 0),
    NumberOfSections(mData, 2),
    Timestamp(mData, 4),
    OptionalHeaderSize(mData, 16)
    {
        mData = new byte[PISTON_PE_IMAGE_FILE_HEADER_SIZE];
    }

    size_t ImageFileHeader::Size() const 
    {
        return PISTON_PE_IMAGE_FILE_HEADER_SIZE;
    }

    Header::PointerType ImageFileHeader::Data() 
    {
        return mData;
    }

    Header::ConstPointerType ImageFileHeader::Data() const 
    {
        return mData;
    }

    ImageFileHeader::~ImageFileHeader()
    {
        delete[] mData;
    }
}