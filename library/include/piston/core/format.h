#ifndef PISTON_CORE_FORMAT_H
#define PISTON_CORE_FORMAT_H

#include <string>
#include <sstream>

namespace piston
{
    template<typename T>
    std::wstring wformat(std::wstringstream* stream, T last)
    {
        (*stream) << last;
        return stream->str();
    }

    template<typename T, typename ...Args>
    std::wstring wformat(std::wstringstream* stream, T first, Args... rest)
    {
        (*stream) << first;
        return wformat(stream, rest...);
    }

    template<typename T>
    std::wstring wformat(T first)
    {
        std::wstringstream stream;
        stream << first;
        return stream.str();
    }

    template<typename T, typename ...Args>
    std::wstring wformat(T first, Args... rest)
    {
        std::wstringstream stream;
        return wformat(&stream, first, rest...);
    }

    template<typename T>
    std::string format(std::stringstream* stream, T last)
    {
        (*stream) << last;
        return stream->str();
    }

    template<typename T, typename ...Args>
    std::string format(std::stringstream* stream, T first, Args... rest)
    {
        (*stream) << first;
        return format(stream, rest...);
    }

    template<typename T>
    std::string format(T first)
    {
        std::stringstream stream;
        stream << first;
        return stream.str();
    }
    
    template<typename T, typename ...Args>
    std::string format(T first, Args... rest)
    {
        std::stringstream stream;
        return format(&stream, first, rest...);
    }
}

#endif // PISTON_CORE_FORMAT_H