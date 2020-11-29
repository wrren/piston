#ifndef PISTON_CORE_CONVERT_H
#define PISTON_CORE_CONVERT_H

#include <string>

namespace piston
{
    /**
     * @brief Type conversion utility class.
     * 
     */
    class convert
    {
    public:

        /**
         * @brief Convert a string to a wide-character string
         * 
         * @param string Input string
         * @return std::wstring Output wide character string
         */
        static std::wstring to_wstring(const std::string& string);
    };
}

#endif // PISTON_CORE_CONVERT_H