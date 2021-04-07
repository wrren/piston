#ifndef PISTON_CORE_STRING_H
#define PISTON_CORE_STRING_H

#include <piston/core/types.h>

namespace Piston
{
    /**
     * @brief Provides utility functions for manipulating strings.
     * 
     */
    class Strings
    {
    public:

        /**
         * @brief Given a set of iterators that bound a set of strings to be joined, join them into one string using the given delimiter.
         * 
         * @tparam C String character type
         * @tparam I Iterator type
         * @param StartIterator Start iterator
         * @param EndIterator End iterator
         * @param Delimiter Delimiter character
         * @return std::basic_string<C> Joined string
         */
        template<typename C, typename I>
        static std::basic_string<C> Join(I StartIterator, I EndIterator, C Delimiter)
        {
            std::basic_stringstream<C> Stream;
            for(auto Iterator = StartIterator; Iterator != EndIterator; Iterator++)
            {
                Stream << *Iterator;
                auto Next = Iterator;

                if(++Next != EndIterator)
                {
                    Stream << Delimiter;
                }
            }
            return Stream.str();
        }

        /**
         * @brief Convert a single-byte character string to a wide string
         * 
         * @param Input Input String
         * @return WideString 
         */
        static WideString StringToWideString(const String& Input);

        /**
         * @brief Convert a wide-character string to a single-byte character string
         * 
         * @param Input 
         * @return String 
         */
        static String WideStringToString(const WideString& Input);
    };
} // namespace Piston


#endif // PISTON_CORE_STRING_H