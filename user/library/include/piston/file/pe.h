#ifndef PISTON_FILE_PE_H
#define PISTON_FILE_PE_H

#include <piston/file/file.h>
#include <piston/file/header/pe/dos.h>
#include <piston/file/header/pe/image_file.h>
#include <piston/file/header/pe/optional.h>

namespace Piston
{
    class PEFile : public File
    {
    public:

        /**
         * @brief Construct a new pe file object
         * 
         */
        PEFile();

        /**
         * @brief Construct a new pe file read from the given path
         * 
         * @param path 
         */
        PEFile(const Path& path);

        /**
         * @brief Determine whether this object represents a valid PE file
         * 
         * @return true If this object represents a valid PE file
         * @return false Otherwise
         */
        bool IsValid() const;

        /**
         * @brief Get the DOS header for this PE file
         * 
         * @return const DOSHeader& DOS header
         */
        const DOSHeader& GetDOSHeader() const;

        /**
         * @brief Get the PE image file header object
         * 
         * @return const image_file_header& 
         */
        const ImageFileHeader& GetImageFileHeader() const;

        /**
         * @brief Get the image optional header object
         * 
         * @return const ImageOptionalHeader& 
         */
        const ImageOptionalHeader& GetImageOptionalHeader() const;

        /**
         * @brief Deserialize PE Format data from the given stream
         * 
         * @param InputStream Input Stream
         * @return true If PE Format data was deserialized successfully.
         * @return false Otherwise
         */
        virtual bool Deserialize(Stream& InputStream) override;

    private:

        // File DOS Header
        DOSHeader mDOSHeader;
        // Image file header
        ImageFileHeader mImageFileHeader;
        // Image optional header (Image files only)
        ImageOptionalHeader mImageOptionalHeader;
    };
}

#endif // PISTON_FILE_PE_H