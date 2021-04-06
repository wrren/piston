#include <piston/process/process.h>

namespace Piston
{
	Process::Process()
	{
	}

	Process::Process(Process::IDType id, const Process::NameType &name) : mID(id),
																			mName(name)
	{
	}

	Process::IDType Process::GetID() const
	{
		return mID;
	}

	bool Process::ReadMemory(const MemoryRegion& region, void* buffer, size_t& bytes_read)
	{
		return ReadMemory(region.GetStartAddress(), buffer, region.GetSize(), bytes_read);
	}

	const Process::NameType &Process::GetName() const
	{
		return mName;
	}

	bool Process::operator==(Process::IDType process_id) const
	{
		return mID == process_id;
	}

	const char *Process::ListException::what() const noexcept
	{
		return "An exception occurred while attempting to enumerate running processes.";
	}
} // namespace Piston