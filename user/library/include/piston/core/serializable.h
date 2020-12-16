#ifndef PISTON_CORE_SERIALIZABLE_H
#define PISTON_CORE_SERIALIZABLE_H

#include <istream>
#include <ostream>

namespace piston
{
    class serializable
    {
    public:

        /**
         * @brief Deserialize data from the given stream
         * 
         * @param stream Input stream
         * @return true If this object was deserialized correctly
         * @return false Otherwise
         */
        virtual bool deserialize(std::istream& stream);

        /**
         * @brief Serialize data into the given stream
         * 
         * @param stream Output stream
         * @return true If this object was serialized correctly
         * @return false Otherwise
         */
        virtual bool serialize(std::ostream& stream);
    };
}

#endif // PISTON_CORE_SERIALIZABLE_H