#ifndef PISTON_CORE_FORMAT_H
#define PISTON_CORE_FORMAT_H

#include <string>
#include <sstream>

namespace Piston
{
    template<typename T>
    std::wstring WideFormat(std::wstringstream* stream, T last)
    {
        (*stream) << last;
        return stream->str();
    }

    template<typename T, typename ...Args>
    std::wstring WideFormat(std::wstringstream* stream, T first, Args... rest)
    {
        (*stream) << first;
        return WideFormat(stream, rest...);
    }

    template<typename T>
    std::wstring WideFormat(T first)
    {
        std::wstringstream stream;
        stream << first;
        return stream.str();
    }

    template<typename T, typename ...Args>
    std::wstring WideFormat(T first, Args... rest)
    {
        std::wstringstream stream;
        return WideFormat(&stream, first, rest...);
    }

    template<typename T>
    std::string Format(std::stringstream* stream, T last)
    {
        (*stream) << last;
        return stream->str();
    }

    template<typename T, typename ...Args>
    std::string Format(std::stringstream* stream, T first, Args... rest)
    {
        (*stream) << first;
        return Format(stream, rest...);
    }

    template<typename T>
    std::string Format(T first)
    {
        std::stringstream stream;
        stream << first;
        return stream.str();
    }
    
    template<typename T, typename ...Args>
    std::string Format(T first, Args... rest)
    {
        std::stringstream stream;
        return Format(&stream, first, rest...);
    }
}

#endif // PISTON_CORE_FORMAT_H