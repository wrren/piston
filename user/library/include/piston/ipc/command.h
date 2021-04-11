#ifndef PISTON_IPC_COMMAND_H
#define PISTON_IPC_COMMAND_H

namespace Piston::IPC
{
    enum Commands
    {
        CONNECT,
        LOG_MESSAGE,
        SCAN_REQUEST,
        SCAN_RESPONSE,
        WRITE_REQUEST,
        WRITE_RESPONSE
    };
}

#endif // PISTON_IPC_COMMAND_H