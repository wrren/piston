#include <piston/core/time.h>

namespace Piston
{
    Time::Timestamp Time::GetTimestamp()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
        ).count();
    }
}