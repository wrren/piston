#ifndef PISTON_CORE_STRING_H
#define PISTON_CORE_STRING_H

#include <string>

namespace piston
{
    /**
     * @brief Provides utility functions for manipulating strings.
     * 
     */
    class strings
    {
    public:

        /**
         * @brief Given a set of iterators that bound a set of strings to be joined, join them into one string using the given delimiter.
         * 
         * @tparam C String character type
         * @tparam I Iterator type
         * @param start_iterator Start iterator
         * @param end_iterator End iterator
         * @param delimiter Delimiter character
         * @return std::basic_string<C> Joined string
         */
        template<typename C, typename I>
        static std::basic_string<C> join(I start_iterator, I end_iterator, C delimiter)
        {
            std::basic_stringstream<C> stream;
            for(auto it = start_iterator; it != end_iterator; it++)
            {
                stream << *it;
                auto next = it;

                if(++next != end_iterator)
                {
                    stream << delimiter;
                }
            }
            return stream.str();
        }
    };
} // namespace piston


#endif // PISTON_CORE_STRING_H