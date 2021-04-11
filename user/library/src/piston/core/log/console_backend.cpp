#include <piston/core/log/console_backend.h>
#include <iostream>

namespace Piston
{
    void ConsoleBackend::Write(const Log::EntryType& NewEntry)
    {
        std::cout   << "[" << std::setw(10) << NewEntry.Timestamp << "] "
                    << std::setw(6) << static_cast<uint32_t>(NewEntry.Level) << " "
                    << NewEntry.Message << std::endl;
    }
}