#include <piston/core/serializable.h>

namespace Piston
{
    bool Serializable::Deserialize(std::istream& stream)
    {
        return stream.good();
    }

    bool Serializable::Serialize(std::ostream& stream) const
    {
        return stream.good();
    }
}