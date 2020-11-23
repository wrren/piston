#include <piston/core/serializable.h>

namespace piston
{
    bool serializable::deserialize(std::istream& stream)
    {
        return stream.good();
    }

    bool serializable::serialize(std::ostream& stream)
    {
        return stream.good();
    }
}