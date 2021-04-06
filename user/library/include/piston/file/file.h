#ifndef PISTON_FILE_H
#define PISTON_FILE_H

#include <piston/core/core.h>

namespace Piston
{
    class File : public Serializable
    {
    public:
        typedef std::shared_ptr<File> PointerType;

        /**
         * @brief Construct a new file object
         * 
         */
        File();

        /**
         * @brief Construct a new file object with the specified path
         * 
         * @param path Path to the file
         */
        File(const Path& path);

        /**
         * @brief Get the path to this file.
         * 
         * @return const path& Path to this file.
         */
        const Path& GetPath() const;

        /**
         * @brief Assign the given path to this file.
         * 
         * @param path New path
         * @return path& 
         */
        File& operator=(const Path& path);

        /**
         * @brief Read data from the file. Calls Deserialize() using the file's file stream, child classes should
         * override the Deserialize() method in order to implement specific read behaviour.
         * 
         * @return true If file deserialization succeeded.
         * @return false Otherwise.
         */
        bool Read();

        /**
         * @brief Write data to the file. Calls Serialize() using the file's file stream, child classes should override
         * the Serialize() method in order to implement specific write behaviour
         * 
         * @return true If file serialization succeeded.
         * @return false Otherwise.
         */
        bool Write();

    private:

        // Path to this file
        Path mPath;
    };
}

#endif // PISTON_FILE_H