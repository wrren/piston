#ifndef PISTON_CORE_SERIALIZABLE_H
#define PISTON_CORE_SERIALIZABLE_H

#include <istream>
#include <ostream>

namespace Piston
{
    class Serializable
    {
    public:

        /**
         * @brief Deserialize data from the given stream
         * 
         * @param stream Input stream
         * @return true If this object was deserialized correctly
         * @return false Otherwise
         */
        virtual bool Deserialize(std::istream& stream);

        /**
         * @brief Serialize data into the given stream
         * 
         * @param stream Output stream
         * @return true If this object was serialized correctly
         * @return false Otherwise
         */
        virtual bool Serialize(std::ostream& stream) const;
    };
}

#endif // PISTON_CORE_SERIALIZABLE_H