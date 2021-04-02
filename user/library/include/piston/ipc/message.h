#ifndef PISTON_IPC_MESSAGE_H
#define PISTON_IPC_MESSAGE_H

#include <piston/core/types.h>
#include <piston/process/process.h>

namespace piston::ipc
{
    class message : public serializable
    {
    public:

        /**
         * @brief Deserialize data from the given stream
         * 
         * @param stream Input stream
         * @return true If this object was deserialized correctly
         * @return false Otherwise
         */
        virtual bool deserialize(std::istream& stream) override;

        /**
         * @brief Serialize data into the given stream
         * 
         * @param stream Output stream
         * @return true If this object was serialized correctly
         * @return false Otherwise
         */
        virtual bool serialize(std::ostream& stream) const override;

    private:

        // Source process ID
        process::id_type m_process;
        // Message sequence number
        uint32_t m_sequence;
        // Internal data buffer
        std::vector<byte> m_data;
    };
}

#endif // PISTON_IPC_MESSAGE_H