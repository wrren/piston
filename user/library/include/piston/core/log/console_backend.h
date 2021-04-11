#ifndef PISTON_CORE_LOG_CONSOLE_BACKEND_H
#define PISTON_CORE_LOG_CONSOLE_BACKEND_H

#include <piston/core/log.h>

namespace Piston
{
    /**
     * @brief Log back-end that writes to the console/stdout
     * 
     */
    class ConsoleBackend : public Log::Backend
    {
    public:

        /**
         * @brief Write a log entry to the console.
         * 
         * @param Entry Log entry
         */
        virtual void Write(const Log::EntryType& Entry) override;
    };
}

#endif // PISTON_CORE_LOG_CONSOLE_BACKEND_H