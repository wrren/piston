#include <piston/file/header/pe/dos.h>
#include <cstdlib>

namespace Piston
{
    const uint16_t DOSHeader::kMagic = 0x5A4D;

    DOSHeader::DOSHeader() :
    eMagic(mData, 0),
    eIFANew(mData, 60)
    {
        memset(mData, 0, DOS_HEADER_SIZE);
    }

    bool DOSHeader::Deserialize(Stream& InputStream)
    {
        return InputStream.ReadBuffer(mData, DOS_HEADER_SIZE);
    }

    bool DOSHeader::Serialize(Stream& OutputStream) const
    {
        return OutputStream.WriteBuffer(mData, DOS_HEADER_SIZE);
    }

    bool DOSHeader::IsValid() const
    {
        return eMagic == kMagic;
    }

    size_t DOSHeader::Size() const 
    {
        return DOS_HEADER_SIZE;
    }

    Header::PointerType DOSHeader::Data() 
    {
        return &(mData[0]);
    }

    Header::ConstPointerType DOSHeader::Data() const 
    {
        return &(mData[0]);
    }

}