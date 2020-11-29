#include <piston/code/breakpoint.h>
#include <algorithm>

namespace piston
{
    breakpoint::list breakpoint::g_breakpoints;
    std::mutex breakpoint::g_mutex;

    breakpoint::check_exception::check_exception(const char* message) :
    m_message(message)
    {}

    const char* breakpoint::check_exception::what() const noexcept
    {
        return m_message.c_str();
    }

    breakpoint::set_exception::set_exception(const char* message) :
    m_message(message)
    {}

    const char* breakpoint::set_exception::what() const noexcept
    {
        return m_message.c_str();
    }

    breakpoint::breakpoint(handler_address handler, breakpoint_address breakpoint, size_type size, register_number register_number, break_type type) :
    m_handler(handler),
    m_address(breakpoint),
    m_size(size),
    m_register(register_number),
    m_type(type),
    m_scope(breakpoint::scope::BP_SCOPE_GLOBAL)
    {
        std::lock_guard<std::mutex> guard(g_mutex);
        g_breakpoints.push_back(*this);
    }

    breakpoint::breakpoint(handler_address handler, breakpoint_address breakpoint, size_type size, register_number register_number, break_type type, platform::thread_id thread_id) :
    m_thread(thread_id),
    m_handler(handler),
    m_address(breakpoint),
    m_size(size),
    m_register(register_number),
    m_type(type),
    m_scope(breakpoint::scope::BP_SCOPE_THREAD)
    {
        std::lock_guard<std::mutex> guard(g_mutex);
        g_breakpoints.push_back(*this);
    }

    void breakpoint::hit()
    {
        std::lock_guard<std::mutex> guard(g_mutex);
        g_breakpoints.erase(std::remove(g_breakpoints.begin(), g_breakpoints.end(), *this), g_breakpoints.end());
        unset(m_register, breakpoint::scope::BP_SCOPE_GLOBAL);
        unset(m_register, breakpoint::scope::BP_SCOPE_THREAD);
    }

    bool breakpoint::operator==(const breakpoint& other) const
    {
        return  m_scope         == other.m_scope   &&
                m_handler       == other.m_handler      &&
                m_address    == other.m_address   &&
                m_register      == other.m_register     &&
                m_size          == other.m_size;
    }
}