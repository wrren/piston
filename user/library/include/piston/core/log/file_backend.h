#ifndef PISTON_CORE_LOG_File_BACKEND_H
#define PISTON_CORE_LOG_File_BACKEND_H

#include <piston/core/log.h>
#include <fstream>

namespace Piston
{
    /**
     * @brief Log back-end that writes to a file.
     * 
     */
    class FileBackend : public Log::Backend
    {
    public:

        /**
         * @brief Construct a new File Backend object
         * 
         * @param FilePath Path to the log file.
         */
        FileBackend(const Path& FilePath);

        /**
         * @brief Write a log entry to the output file.
         * 
         * @param Entry Log entry
         */
        virtual void Write(const Log::EntryType& Entry) override;

    private:

        // File stream
        std::ofstream mOutputStream;
    };
}

#endif // PISTON_CORE_LOG_File_BACKEND_H