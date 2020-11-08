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
    void list(std::wostream& output_stream, std::ostream& error_stream);

    /**
     * @brief Attempt to inject the library at the given path into the process with the given ID
     * 
     * @param output_stream Output stream to which injection operation messages should be sent
     * @param error_stream Output stream to which error messages should be sent
     * @param library_path Path to the library to be injected
     * @param process_id ID of the process into which the library should be injected
     */
    void inject_into_process(std::ostream& output_stream, std::ostream& error_stream, const piston::injector::library_path& library_path, piston::process::id_type process_id);
}

#endif // PISTON_CLI_OPERATIONS_H