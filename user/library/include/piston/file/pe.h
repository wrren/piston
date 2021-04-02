#ifndef PISTON_FILE_PE_H
#define PISTON_FILE_PE_H

#include <piston/file/file.h>
#include <piston/file/header/pe/dos.h>
#include <piston/file/header/pe/image_file.h>
#include <piston/file/header/pe/optional.h>

namespace piston
{
    class pe_file : public file
    {
    public:

        /**
         * @brief Construct a new pe file object
         * 
         */
        pe_file();

        /**
         * @brief Construct a new pe file read from the given path
         * 
         * @param path 
         */
        pe_file(const path& path);

        /**
         * @brief Determine whether this object represents a valid PE file
         * 
         * @return true If this object represents a valid PE file
         * @return false Otherwise
         */
        bool is_valid() const;

        /**
         * @brief Get the DOS header for this PE file
         * 
         * @return const dos_header& DOS header
         */
        const dos_header& get_dos_header() const;

        /**
         * @brief Get the PE image file header object
         * 
         * @return const image_file_header& 
         */
        const image_file_header& get_image_file_header() const;

        /**
         * @brief Get the image optional header object
         * 
         * @return const image_optional_header& 
         */
        const image_optional_header& get_image_optional_header() const;

        /**
         * @brief Deserialize PE Format data from the given stream
         * 
         * @param stream Input Stream
         * @return true If PE Format data was deserialized successfully.
         * @return false Otherwise
         */
        virtual bool deserialize(std::istream& stream) override;

    private:

        // File DOS Header
        dos_header m_dos_header;
        // Image file header
        image_file_header m_image_file_header;
        // Image optional header (Image files only)
        image_optional_header m_image_optional_header;
    };
}

#endif // PISTON_FILE_PE_H