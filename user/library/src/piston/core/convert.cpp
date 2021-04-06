#include <piston/core/convert.h>

namespace Piston
{
    std::wstring Convert::ToWideString(const std::string& string)
    {
        return std::wstring(string.begin(), string.end());
    }
}