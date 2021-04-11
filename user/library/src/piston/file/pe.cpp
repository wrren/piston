#include <piston/file/pe.h>

#define PISTON_PE_NT_SIGNATURE 0x00004550

namespace Piston
{
    PEFile::PEFile()
    {}

    PEFile::PEFile(const Path& path) :
    File(path)
    {}

    bool PEFile::IsValid() const
    {
        return mDOSHeader.IsValid();
    }

    const DOSHeader& PEFile::GetDOSHeader() const 
    {
        return mDOSHeader;
    }

    const ImageFileHeader& PEFile::GetImageFileHeader() const
    {
        return mImageFileHeader;
    }

    const ImageOptionalHeader& PEFile::GetImageOptionalHeader() const
    {
        return mImageOptionalHeader;
    }

    bool PEFile::Deserialize(Stream& InputStream)
    {
        if(mDOSHeader.Deserialize(InputStream))
        {
            InputStream.Seek(mDOSHeader.eIFANew.GetValue());
            uint32_t nt_signature;
            InputStream.ReadBuffer(reinterpret_cast<byte*>(&nt_signature), sizeof(nt_signature));

            if(nt_signature == PISTON_PE_NT_SIGNATURE && mImageFileHeader.Deserialize(InputStream))
            {
                mImageOptionalHeader.SetSize(mImageFileHeader.OptionalHeaderSize.GetValue());
                return mImageOptionalHeader.Deserialize(InputStream);
            }            
        }
        return false;
    }
}