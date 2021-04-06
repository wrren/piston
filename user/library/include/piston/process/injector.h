#ifndef PISTON_PROCESS_INJECTOR_H
#define PISTON_PROCESS_INJECTOR_H

#include <piston/core/types.h>
#include <piston/process/process.h>
#include <filesystem>
#include <exception>

namespace Piston
{
    /**
     * @brief Facilitates injection of shared libraries into processes.
     * 
     */
    class Injector
    {
    public:

        typedef Path LibraryPath;
        typedef Path ExecutablePath;
        typedef std::vector<String> ArgumentList;

        enum class InjectMode
        {
            MODE_INJECT_NEW,
            MODE_INJECT_RUNNING
        };

        class Exception : public std::exception
        {
        public:

            /**
             * @brief Construct a new injection exception object
             * 
             * @param what Error message
             */
            Exception(const std::string& what);

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
        static Injector Inject(const LibraryPath& library_path, Process::IDType process_id);

        /**
         * @brief Attempt to inject the library at the given path into a new instance of the executable at the given path.
         * 
         * @param library_path Path to the library to be injected into the target executable
         * @param executable_path Path to the executable into which the target library should be injected
         * @param arguments Command-line arguments passed to the executable
         * @return true If injection succeeds
         * @return false If injection fails
         */
        static Injector Inject(const LibraryPath& library_path, const ExecutablePath& executable_path, const ArgumentList& args = {});

        /**
         * @brief Get the injection mode for this injection
         * 
         * @return Injection mode 
         */
        InjectMode GetMode() const;

        /**
         * @brief Get the path to the library being injected into the target executable or process. 
         * 
         * @return Path to the library being injected into the target executable or process. 
         */
        const LibraryPath& GetLibraryPath() const;

        /**
         * @brief Get the ID of the process into which a library has been injected.
         * 
         * @return ID of the process into which a library has been injected.
         */
        Process::IDType GetProcessID() const;

    protected:

        /**
         * @brief Construct a new injector object
         * 
         * @param mode Injection mode
         * @param library_path Path to the library being injected into the target executable or process
         * @param process_id Target process for injection
         */
        Injector(InjectMode mode, const LibraryPath& library_path, Process::IDType process_id);

    private:

        /// Injection Mode
        InjectMode mMode;
        /// Path to the library being injected
        LibraryPath mLibraryPath;
        /// ID of the process into which the library is being injected
        Process::IDType mProcessID;
    };
} // namespace Piston

#endif // PISTON_PROCESS_INJECTOR_H