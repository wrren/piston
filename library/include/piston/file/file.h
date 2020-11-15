#ifndef PISTON_FILE_H
#define PISTON_FILE_H

#include <piston/core/types.h>

namespace piston
{
    class file
    {
    public:

        class parse_exception : public std::exception
        {
        public:

            /**
             * @brief Construct a new parse exception object
             * 
             * @param path 
             */
            parse_exception(const path& path);

            /**
             * @brief Construct a new parse exception object
             * 
             * @param message 
             */
            parse_exception(const string& message);

            /**
             * @brief Return a human-readable description of this exception
             * 
             * @return const char* 
             */
            virtual const char* what() const override;

        private:

            string m_message;

            path m_path;
        };

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
        virtual path& operator=(const path& path);

    private:

        // Path to this file
        path m_path;
    };
}

#endif // PISTON_FILE_H