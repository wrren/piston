#ifndef PISTON_FILE_H
#define PISTON_FILE_H

#include <piston/core/core.h>

namespace piston
{
    class file : public serializable
    {
    public:
        typedef std::shared_ptr<file> ptr_type;

        /**
         * @brief Construct a new file object
         * 
         */
        file();

        /**
         * @brief Construct a new file object with the specified path
         * 
         * @param path Path to the file
         */
        file(const path& path);

        /**
         * @brief Get the path to this file.
         * 
         * @return const path& Path to this file.
         */
        const path& get_path() const;

        /**
         * @brief Assign the given path to this file.
         * 
         * @param path New path
         * @return path& 
         */
        file& operator=(const path& path);

        /**
         * @brief Read data from the file. Calls deserialize() using the file's file stream, child classes should
         * override the deserialize() method in order to implement specific read behaviour.
         * 
         * @return true If file deserialization succeeded.
         * @return false Otherwise.
         */
        bool read();

        /**
         * @brief Write data to the file. Calls serialize() using the file's file stream, child classes should override
         * the serialize() method in order to implement specific write behaviour
         * 
         * @return true If file serialization succeeded.
         * @return false Otherwise.
         */
        bool write();

    private:

        // Path to this file
        path m_path;
    };
}

#endif // PISTON_FILE_H