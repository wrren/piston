#ifndef PISTON_CORE_SERIALIZABLE_H
#define PISTON_CORE_SERIALIZABLE_H

#include <piston/core/stream.h>

namespace Piston
{
    class Serializable
    {
    public:

        /**
         * @brief Deserialize data from the given stream
         * 
         * @param InputStream Input stream
         * @return true If this object was deserialized correctly
         * @return false Otherwise
         */
        virtual bool Deserialize(Stream& InputStream);

        /**
         * @brief Serialize data into the given stream
         * 
         * @param OutputStream Output stream
         * @return true If this object was serialized correctly
         * @return false Otherwise
         */
        virtual bool Serialize(Stream& OutputStream) const;
    };
}

#endif // PISTON_CORE_SERIALIZABLE_H