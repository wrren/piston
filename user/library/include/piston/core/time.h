#ifndef PISTON_CORE_TIME_H
#define PISTON_CORE_TIME_H

#include <piston/core/types.h>

namespace Piston
{
    class Time
    {
    public:

        typedef uint64_t Timestamp;

        /**
         * @brief Get the current time as a timestamp
         * 
         * @return Timestamp 
         */
        static Timestamp GetTimestamp();
    };
}

#endif // PISTON_CORE_TIME_H