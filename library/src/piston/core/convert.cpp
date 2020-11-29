#include <piston/core/convert.h>

namespace piston
{
    std::wstring convert::to_wstring(const std::string& string)
    {
        return std::wstring(string.begin(), string.end());
    }
}