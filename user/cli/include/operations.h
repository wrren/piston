#ifndef PISTON_CLI_OPERATIONS_H
#define PISTON_CLI_OPERATIONS_H

#include <piston/process/process.h>
#include <piston/process/injector.h>
#include <ostream>

namespace Piston::cli
{
    /**
     * @brief List running processes
     * 
     * @param output_stream Output stream to which list outputs should be sent
     * @param error_stream Output stream to which error messages should be sent
     */
    void list(std::ostream& output_stream, std::ostream& error_stream);

    /**
     * @brief Dynamically load the library at the given path.
     * 
     * @param output_stream Output stream to which list outputs should be sent
     * @param error_stream Output stream to which error messages should be sent
     * @param library_path Path to the library to be loaded.
     */
    void load(std::ostream& output_stream, std::ostream& error_stream, const Piston::Path& library_path);

    /**
     * @brief Attempt to inject the library at the given path into the process with the given ID
     * 
     * @param output_stream Output stream to which injection operation messages should be sent
     * @param error_stream Output stream to which error messages should be sent
     * @param library_path Path to the library to be injected
     * @param process_id ID of the process into which the library should be injected
     */
    void inject_into_process(
        std::ostream& output_stream, 
        std::ostream& error_stream, 
        const Piston::Injector::LibraryPath& library_path, 
        Piston::Process::IDType process_id);

    /**
     * @brief Attempt to inject a library at the given path into a new instance of the executable at the given path.
     * 
     * @param output_stream Output stream to which injection operation messages should be sent
     * @param error_stream Output stream to which error messages should be sent
     * @param library_path Path to the library to be injected
     * @param executable_path Path to the executable into which the given library is to be injected
     * @param arguments Arguments to the executable being started
     */
    void inject_into_executable(
        std::ostream& output_stream, 
        std::ostream& error_stream, 
        const Piston::Injector::LibraryPath& library_path, 
        const Piston::Injector::ExecutablePath& executable_path,
        const Piston::Injector::ArgumentList& arguments = {});

    /**
     * @brief Attempt to read the given file as a PE file
     * 
     * @param output_stream Output stream to which injection operation messages should be sent
     * @param error_stream Output stream to which error messages should be sent
     * @param path Path to the file
     */
    void read_file(std::ostream& output_stream, std::ostream& error_stream, const Piston::Path path);
}

#endif // PISTON_CLI_OPERATIONS_H