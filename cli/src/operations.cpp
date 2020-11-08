#include <operations.h>
#include <piston/process/injector.h>

namespace piston::cli
{
    void list(std::wostream& output_stream, std::ostream& error_stream)
    {
        piston::process::list_type processes;

        try
        {
            processes = piston::process::list();
        }
        catch (std::exception& e)
        {
            error_stream << "An error occurred while listing running processes: " << e.what() << std::endl;
        }

        for (auto process : processes)
        {
            output_stream << std::setw(10) << std::left << process->get_id() << process->get_name() << std::endl;
        }
    }

    void inject_into_process(std::ostream& output_stream, std::ostream& error_stream, const piston::injector::library_path& library_path, piston::process::id_type process_id)
    {
        try
        {
            auto injector = injector::inject(library_path, process_id);
            output_stream << "Library injected into process ID" << process_id << std::endl;
        }
        catch(const std::exception& e)
        {
            error_stream << e.what() << std::endl;
        }
    }
}