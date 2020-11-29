#include <piston/process/injector.h>

namespace piston
{
    injector::injector(injector::mode mode, const injector::library_path& library_path, process::id_type process_id) :
    m_mode(mode),
    m_library_path(library_path),
    m_process_id(process_id)
    {}

    injector::mode injector::get_mode() const
    {
        return m_mode;
    }

    process::id_type injector::get_process_id() const
    {
        return m_process_id;
    }

    const injector::library_path& injector::get_library_path() const
    {
        return m_library_path;
    }

    injector::injection_exception::injection_exception(const std::string& what) :
    m_what(what)
    {}

    const char* injector::injection_exception::what() const noexcept
    {
        return m_what.c_str();
    }
}