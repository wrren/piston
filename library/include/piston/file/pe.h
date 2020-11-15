#ifndef PISTON_FILE_PE_H
#define PISTON_FILE_PE_H

#include <piston/file/file.h>

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
    };
}

#endif // PISTON_FILE_PE_H