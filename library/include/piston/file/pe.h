#ifndef PISTON_FILE_PE_H
#define PISTON_FILE_PE_H

#include <piston/file/file.h>
#include <piston/file/header/dos.h>

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
    };
}

#endif // PISTON_FILE_PE_H