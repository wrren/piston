#ifndef PISTON_CORE_CONVERT_H
#define PISTON_CORE_CONVERT_H

#include <string>

namespace Piston
{
    /**
     * @brief Type conversion utility class.
     * 
     */
    class Convert
    {
    public:

        /**
         * @brief Convert a string to a wide-character string
         * 
         * @param string Input string
         * @return std::wstring Output wide character string
         */
        static std::wstring ToWideString(const std::string& string);
    };
}

#endif // PISTON_CORE_CONVERT_H