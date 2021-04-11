#include <piston/ipc/messages/scan_request.h>

namespace Piston::IPC
{
    const Message::CommandType ScanRequest::Command = Commands::SCAN_REQUEST;

    ScanRequest::ScanRequest() :
    Message(Command),
    mValueSize(0),
    mTargetValue(nullptr)
    {}

    ScanRequest::ScanRequest(ScanType Type, const byte* TargetValue, size_t ValueSize, const MemoryRegion::ListType& TargetRegions) :
    Message(Command),
    mType(Type),
    mValueSize(ValueSize),
    mTargetRegions(TargetRegions)
    {
        mTargetValue = new byte[ValueSize];
        memcpy(mTargetValue, TargetValue, ValueSize);
    }

    void ScanRequest::SetScanRegions(const MemoryRegion::ListType& Regions)
    {
        mTargetRegions = Regions;
    }

    void ScanRequest::RegisterWith(MessageFactory::PointerType Factory)
    {
        Factory->RegisterMessageMaker(ScanRequest::Command, []()
        {
            return Message::PointerType(new ScanRequest());
        });
    }

    bool ScanRequest::Deserialize(Stream& InputStream)
    {
        uint8_t ScanType;
        InputStream >> ScanType >> mValueSize;
        mType = static_cast<ScanRequest::ScanType>(ScanType);

        if(mTargetValue != nullptr)
        {
            delete[] mTargetValue;
        }

        mTargetValue = new byte[mValueSize];

        InputStream.ReadBuffer(mTargetValue, mValueSize);

        uint32_t RegionListSize;
        InputStream >> RegionListSize;

        for(uint32_t i = 0; i < RegionListSize; i++)
        {
            MemoryRegion NewRegion;
            if(NewRegion.Deserialize(InputStream))
            {
                mTargetRegions.push_back(NewRegion);
            }
            else
            {
                break;
            }
        }
        return InputStream.Good();
    }

    bool ScanRequest::Serialize(Stream& OutputStream) const
    {
        OutputStream << static_cast<uint8_t>(mType) << mValueSize;
        OutputStream.WriteBuffer(mTargetValue, mValueSize);
        
        OutputStream << static_cast<uint32_t>(mTargetRegions.size());
        for(auto& Entry : mTargetRegions)
        {
            if(!Entry.Serialize(OutputStream))
            {
                break;
            }
        }

        return OutputStream.Good();
    }

    ScanRequest::ScanType ScanRequest::GetScanType() const
    {
        return mType;
    }

    MemoryRegion::ListType ScanRequest::ListTargetRegions() const
    {
        return mTargetRegions;
    }

    const byte* const ScanRequest::GetTargetValue() const
    {
        return mTargetValue;
    }

    const size_t ScanRequest::GetTargetValueSize() const
    {
        return mValueSize;
    }
    
    ScanRequest::~ScanRequest()
    {
        if(mTargetValue != nullptr)
        {
            delete[] mTargetValue;
        }
    }
}