#include <piston/process/injector.h>

namespace Piston
{
    Injector::Injector(Injector::InjectMode mode, const Injector::LibraryPath& library_path, Process::IDType process_id) :
    mMode(mode),
    mLibraryPath(library_path),
    mProcessID(process_id)
    {}

    Injector::InjectMode Injector::GetMode() const
    {
        return mMode;
    }

    Process::IDType Injector::GetProcessID() const
    {
        return mProcessID;
    }

    const Injector::LibraryPath& Injector::GetLibraryPath() const
    {
        return mLibraryPath;
    }

    Injector::Exception::Exception(const std::string& what) :
    m_what(what)
    {}

    const char* Injector::Exception::what() const noexcept
    {
        return m_what.c_str();
    }
}