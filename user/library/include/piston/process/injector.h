#ifndef PISTON_PROCESS_INJECTOR_H
#define PISTON_PROCESS_INJECTOR_H

#include <piston/process/process.h>
#include <filesystem>
#include <exception>

namespace piston
{
    /**
     * @brief Facilitates injection of shared libraries into processes.
     * 
     */
    class injector
    {
    public:

        typedef std::filesystem::path library_path;
        typedef std::filesystem::path executable_path;
        typedef std::vector<std::string> argument_list;

        enum class mode
        {
            MODE_INJECT_NEW,
            MODE_INJECT_RUNNING
        };

        class injection_exception : public std::exception
        {
        public:

            /**
             * @brief Construct a new injection exception object
             * 
             * @param what Error message
             */
            injection_exception(const std::string& what);

            /**
             * @brief Get the human-readable error message for this injection exception
             * 
             * @return Exception message
             */
            virtual const char* what() const noexcept override;

        private:

            std::string m_what;
        };

        /**
         * @brief Attempt to inject the library at the given path into the process with the given ID
         * 
         * @param library_path Path to the library to be injected into the target process
         * @param process_id ID of the process into which the library should be injected
         * @return true If injection succeeds
         * @return false If injection fails
         */
        static injector inject(const library_path& library_path, process::id_type process_id);

        /**
         * @brief Attempt to inject the library at the given path into a new instance of the executable at the given path.
         * 
         * @param library_path Path to the library to be injected into the target executable
         * @param executable_path Path to the executable into which the target library should be injected
         * @param arguments Command-line arguments passed to the executable
         * @return true If injection succeeds
         * @return false If injection fails
         */
        static injector inject(const library_path& library_path, const executable_path& executable_path, const argument_list& args = {});

        /**
         * @brief Get the injection mode for this injection
         * 
         * @return Injection mode 
         */
        mode get_mode() const;

        /**
         * @brief Get the path to the library being injected into the target executable or process. 
         * 
         * @return Path to the library being injected into the target executable or process. 
         */
        const library_path& get_library_path() const;

        /**
         * @brief Get the ID of the process into which a library has been injected.
         * 
         * @return ID of the process into which a library has been injected.
         */
        process::id_type get_process_id() const;

    protected:

        /**
         * @brief Construct a new injector object
         * 
         * @param mode Injection mode
         * @param library_path Path to the library being injected into the target executable or process
         * @param process_id Target process for injection
         */
        injector(mode mode, const library_path& library_path, process::id_type process_id);

    private:

        /// Injection Mode
        mode m_mode;
        /// Path to the library being injected
        library_path m_library_path;
        /// ID of the process into which the library is being injected
        process::id_type m_process_id;
    };
} // namespace piston

#endif // PISTON_PROCESS_INJECTOR_H