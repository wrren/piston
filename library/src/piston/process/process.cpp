#include <piston/process/process.h>

namespace piston
{
	process::process()
	{
	}

	process::process(process::id_type id, const process::name_type &name) : m_id(id),
																			m_name(name)
	{
	}

	process::id_type process::get_id() const
	{
		return m_id;
	}

	const process::name_type &process::get_name() const
	{
		return m_name;
	}

	bool process::operator==(process::id_type process_id) const
	{
		return m_id == process_id;
	}

	const char *process::list_exception::what() const noexcept
	{
		return "An exception occurred while attempting to enumerate running processes.";
	}
} // namespace piston