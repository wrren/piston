#include <operations.h>
#include <piston/file/pe.h>
#include <piston/process/injector.h>
#include <thread>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace Piston::cli
{
    void list(std::ostream& output_stream, std::ostream& error_stream)
    {
        Piston::Process::ListType processes;

        try
        {
            processes = Piston::Process::List();
        }
        catch (std::exception& e)
        {
            error_stream << "An error occurred while listing running processes: " << e.what() << std::endl;
        }

        for (auto process : processes)
        {
            output_stream << std::setw(10) << std::left << process->GetID() << process->GetName() << std::endl;
        }
    }

    void load(std::ostream& output_stream, std::ostream& error_stream, const Piston::Path& library_path)
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

    void inject_into_process(std::ostream& output_stream, std::ostream& error_stream, const Piston::Injector::LibraryPath& library_path, Piston::Process::IDType process_id)
    {
        try
        {
            auto injector = Injector::Inject(library_path, process_id);
            output_stream << "Library injected into process ID " << injector.GetProcessID() << std::endl;
        }
        catch(const std::exception& e)
        {
            error_stream << e.what() << std::endl;
        }
    }

    void inject_into_executable(std::ostream& output_stream, std::ostream& error_stream, const Piston::Injector::LibraryPath& library_path, const Piston::Injector::ExecutablePath& executable_path, const Piston::Injector::ArgumentList& arguments)
    {
        try
        {
            auto injector = Injector::Inject(library_path, executable_path, arguments);
            output_stream << "Library injected into process ID " << injector.GetProcessID() << std::endl;
        }
        catch(const std::exception& e)
        {
            error_stream << e.what() << std::endl;
        }
    }

    void read_file(std::ostream& output_stream, std::ostream& error_stream, const Piston::Path path)
    {
        try
        {
            auto file = PEFile(path);
            
            if(!file.Read())
            {
                error_stream << "Error during file read." << std::endl;
                return;
            }

            if(!file.IsValid())
            {
                error_stream << "PE file invalid. magic value: " << std::hex << file.GetDOSHeader().e_magic << ", expected " << std::hex << file.GetDOSHeader().k_magic << std::endl;
                return;
            }

            output_stream << "PE file valid! PE Offset: " << std::hex << file.GetDOSHeader().e_ifanew << std::endl;
            output_stream << "Number of sections: " << file.GetImageFileHeader().NumberOfSections << std::endl;
            output_stream << "Timestamp: " << file.GetImageFileHeader().Timestamp << std::endl;
            output_stream << "Size of code: " << file.GetImageOptionalHeader().SizeOfCode.GetValue() << std::endl;
        }
        catch(const std::exception& e)
        {
            error_stream << e.what() << std::endl;
        }
    }
}