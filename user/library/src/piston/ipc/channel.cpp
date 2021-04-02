#include <piston/ipc/channel.h>

namespace piston::ipc
{
    channel::channel(process::ptr_type target) :
    m_target(target)
    {}
}