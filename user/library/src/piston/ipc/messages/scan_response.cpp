#include <piston/ipc/messages/scan_response.h>

namespace Piston::IPC
{
    const Message::CommandType ScanResponse::Command = Commands::SCAN_RESPONSE;

    ScanResponse::ScanResponse() :
    Message(Command)
    {}

    ScanResponse::ScanResponse(const MemoryRegion::ListType& Results) :
    Message(Command),
    mResults(Results)
    {}

    void ScanResponse::RegisterWith(MessageFactory::PointerType Factory)
    {
        Factory->RegisterMessageMaker(ScanResponse::Command, []()
        {
            return Message::PointerType(new ScanResponse());
        });
    }

    bool ScanResponse::Deserialize(Stream& InputStream)
    {
        uint32_t RegionListSize;
        InputStream >> RegionListSize;

        for(uint32_t i = 0; i < RegionListSize; i++)
        {
            MemoryRegion NewRegion;
            if(NewRegion.Deserialize(InputStream))
            {
                mResults.push_back(NewRegion);
            }
            else
            {
                PISTON_LOG_ERROR("Failed to deserialize memory region");
                break;
            }
        }

        return InputStream.Good();
    }

    bool ScanResponse::Serialize(Stream& OutputStream) const
    {
        OutputStream << static_cast<uint32_t>(mResults.size());
        
        for(auto& Entry : mResults)
        {
            if(!Entry.Serialize(OutputStream))
            {
                PISTON_LOG_ERROR("Failed to serialize memory region");
                break;
            }
        }

        return OutputStream.Good();
    }

    MemoryRegion::ListType ScanResponse::GetResults() const
    {
        return mResults;
    }
}