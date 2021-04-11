#include <piston/ipc/messages/write_request.h>

namespace Piston::IPC
{
    const Message::CommandType WriteRequest::Command = Commands::WRITE_REQUEST;

    WriteRequest::WriteRequest() :
    Message(Command)
    {}

    WriteRequest::WriteRequest(MemoryRegion::AddressType TargetAddress, const byte* Data, size_t DataSize) :
    Message(Command),
    mTargetAddress(TargetAddress)
    {
        if(DataSize > 0)
        {
            mData = new byte[DataSize];
            mDataSize = DataSize;
            memcpy(mData, Data, DataSize);
        }
    }

    void WriteRequest::RegisterWith(MessageFactory::PointerType Factory)
    {
        Factory->RegisterMessageMaker(WriteRequest::Command, []()
        {
            return Message::PointerType(new WriteRequest());
        });
    }

    bool WriteRequest::Deserialize(Stream& InputStream)
    {
        uint64_t TargetAddress = 0;
        uint32_t DataSize = 0;

        InputStream >> TargetAddress >> DataSize;
        mTargetAddress = TargetAddress;
        mDataSize = DataSize;

        if(mData != nullptr)
        {
            delete[] mData;
        }

        mData = new byte[DataSize];

        InputStream.ReadBuffer(mData, DataSize);

        return InputStream.Good();
    }

    bool WriteRequest::Serialize(Stream& OutputStream) const
    {
        OutputStream << static_cast<uint64_t>(mTargetAddress) << static_cast<uint32_t>(mDataSize);
        OutputStream.WriteBuffer(mData, mDataSize);

        return OutputStream.Good();
    }

    MemoryRegion::AddressType WriteRequest::GetTargetAddress() const
    {
        return mTargetAddress;
    }

    byte* WriteRequest::GetData() const
    {
        return mData;
    }

    size_t WriteRequest::GetDataSize() const
    {
        return mDataSize;
    }

    WriteRequest::~WriteRequest()
    {
        if(mData != nullptr)
        {
            delete[] mData;
        }
    }

}