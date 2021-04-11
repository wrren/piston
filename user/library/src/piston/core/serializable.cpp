#include <piston/core/serializable.h>

namespace Piston
{
    bool Serializable::Deserialize(Stream& InputStream)
    {
        return InputStream.Good();
    }

    bool Serializable::Serialize(Stream& OutputStream) const
    {
        return OutputStream.Good();
    }
}