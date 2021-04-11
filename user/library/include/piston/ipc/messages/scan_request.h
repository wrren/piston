#ifndef PISTON_IPC_SCAN_REQUEST_H
#define PISTON_IPC_SCAN_REQUEST_H

#include <piston/ipc/message.h>
#include <piston/ipc/factory.h>
#include <piston/process/memory/region.h>

namespace Piston::IPC
{
    class ScanRequest : public Message
    {
    public:

        static const Message::CommandType Command;

        enum class ScanType : uint8_t
        {
            FullScan,
            ReScan
        };

        /**
         * @brief Construct a new Scan Request object
         * 
         */
        ScanRequest();

        /**
         * @brief Construct a new Scan Request object
         * 
         * @param Type Scan type
         * @param TargetValue Target value to be scanned for
         * @param ValueSize Size of the value in bytes
         * @param TargetRegion Target memory regions for the scan
         */
        ScanRequest(ScanType Type, const byte* TargetValue, size_t ValueSize, const MemoryRegion::ListType& TargetRegions = {});

        /**
         * @brief Set the target memory regions for this scan.
         * 
         * @param Regions Target regions
         */
        void SetScanRegions(const MemoryRegion::ListType& Regions);

        /**
         * @brief Register this message type with the given message factory.
         * 
         * @param Factory Message factory to register with.
         */
        static void RegisterWith(MessageFactory::PointerType Factory);

        /**
         * @brief Deserialize a  message from the given input stream
         * 
         * @param InputStream Input stream
         * @return true If the message was deserialized successfully
         * @return false Otherwise
         */
        virtual bool Deserialize(Stream& InputStream) override;

        /**
         * @brief Serialize a message into the given output stream
         * 
         * @param OutputStream Output stream
         * @return true If the message was serialized successfully
         * @return false Otherwise
         */
        virtual bool Serialize(Stream& OutputStream) const override;

        /**
         * @brief Get the Scan Type for this request
         * 
         * @return ScanType 
         */
        ScanType GetScanType() const;

        /**
         * @brief Get a list of target memory regions for this scan
         * 
         * @return MemoryRegion::ListType 
         */
        MemoryRegion::ListType ListTargetRegions() const;

        /**
         * @brief Get the Target Value for this scan request
         * 
         * @return const byte* const 
         */
        const byte* const GetTargetValue() const;

        /**
         * @brief Get the Target Value Size
         * 
         * @return const size_t 
         */
        const size_t GetTargetValueSize() const;

        /**
         * @brief Destroy the Scan Request object
         * 
         */
        ~ScanRequest();

    private:

        // Scan type
        ScanType mType;
        // Target scan value
        byte* mTargetValue;
        // Size of the value
        size_t mValueSize;
        // Target regions
        MemoryRegion::ListType mTargetRegions;
    };
}

#endif // PISTON_IPC_SCAN_REQUEST_H