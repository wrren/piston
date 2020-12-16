#ifndef PISTON_CLI_OPERATIONS_H
#define PISTON_CLI_OPERATIONS_H

#include <piston/process/process.h>
#include <piston/process/injector.h>
#include <ostream>

namespace piston::cli
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
    void load(std::ostream& output_stream, std::ostream& error_stream, const piston::path& library_path);

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
        const piston::injector::library_path& library_path, 
        piston::process::id_type process_id);

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
        const piston::injector::library_path& library_path, 
        const piston::injector::executable_path& executable_path,
        const piston::injector::argument_list& arguments = {});

    /**
     * @brief Attempt to read the given file as a PE file
     * 
     * @param output_stream Output stream to which injection operation messages should be sent
     * @param error_stream Output stream to which error messages should be sent
     * @param path Path to the file
     */
    void read_file(std::ostream& output_stream, std::ostream& error_stream, const piston::path path);
}

#endif // PISTON_CLI_OPERATIONS_H