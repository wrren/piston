#include <operations.h>
#include <piston/file/pe.h>
#include <piston/process/injector.h>
#include <thread>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace piston::cli
{
    void list(std::ostream& output_stream, std::ostream& error_stream)
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

    void load(std::ostream& output_stream, std::ostream& error_stream, const piston::path& library_path)
    {
        auto module = LoadLibraryA(library_path.string().c_str());
        if(module == NULL)
        {
            error_stream << "Failed to Load Library: " << GetLastError() << std::endl;
        }
        else
        {
            output_stream << "Library Loaded!" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }

    void inject_into_process(std::ostream& output_stream, std::ostream& error_stream, const piston::injector::library_path& library_path, piston::process::id_type process_id)
    {
        try
        {
            auto injector = injector::inject(library_path, process_id);
            output_stream << "Library injected into process ID " << injector.get_process_id() << std::endl;
        }
        catch(const std::exception& e)
        {
            error_stream << e.what() << std::endl;
        }
    }

    void inject_into_executable(std::ostream& output_stream, std::ostream& error_stream, const piston::injector::library_path& library_path, const piston::injector::executable_path& executable_path, const piston::injector::argument_list& arguments)
    {
        try
        {
            auto injector = injector::inject(library_path, executable_path, arguments);
            output_stream << "Library injected into process ID " << injector.get_process_id() << std::endl;
        }
        catch(const std::exception& e)
        {
            error_stream << e.what() << std::endl;
        }
    }

    void read_file(std::ostream& output_stream, std::ostream& error_stream, const piston::path path)
    {
        try
        {
            auto file = pe_file(path);
            
            if(!file.read())
            {
                error_stream << "Error during file read." << std::endl;
                return;
            }

            if(!file.is_valid())
            {
                error_stream << "PE file invalid. magic value: " << std::hex << file.get_dos_header().e_magic << ", expected " << std::hex << file.get_dos_header().k_magic << std::endl;
                return;
            }


            output_stream << "PE file valid! PE Offset: " << std::hex << file.get_dos_header().e_ifanew << std::endl;
        }
        catch(const std::exception& e)
        {
            error_stream << e.what() << std::endl;
        }
    }
}