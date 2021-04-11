#include <piston/core/log/file_backend.h>
#include <iostream>

namespace Piston
{
    FileBackend::FileBackend(const Path& FilePath)
    {
        mOutputStream = std::ofstream(FilePath);
    }

    void FileBackend::Write(const Log::EntryType& NewEntry)
    {
        mOutputStream   << "[" << std::setw(10) << NewEntry.Timestamp << "] "
                        << std::setw(6) << static_cast<uint32_t>(NewEntry.Level) << " "
                        << NewEntry.Message << std::endl;
    }
}