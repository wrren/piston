#ifndef PISTON_PROCESS_H
#define PISTON_PROCESS_H

#include <piston/core/core.h>
#include <memory>
#include <vector>
#include <optional>

namespace piston
{
    class process
    {
    public:

        typedef uintptr_t base_address;
        typedef piston::platform::process_id id_type;
        typedef piston::platform::process_name name_type;
        typedef std::shared_ptr<process> ptr_type;
        typedef std::vector<ptr_type> list_type;

        /**
         * @brief Exception type thrown when a failure occurs during process listing.
         * 
         */
        class list_exception : public std::exception
        {
        public:

            virtual const char* what() const noexcept override;
        };

        /**
         * @brief Construct a new process object
         * 
         */
        process();

		/**
		 * @brief Construct a new process object
		 * 
		 * @param id process ID
		 * @param name process name
		 */
		process(id_type id, const name_type& name);

		/**
		 * @brief Get the ID of this process.
		 * 
		 * @return Process ID
		 */
		id_type get_id() const;

        /**
         * @brief Get the base address of this process
         * 
         * @return base_address Process base address
         */
        std::optional<base_address> get_base_address() const;

        /**
         * @brief Read data from this process' memory
         * 
         * @param address Address from which the data should be read
         * @param buffer Buffer into which the data should be written
         * @param bytes Number of bytes to read
         * @param bytes_read Number of bytes that were read
         * @return true If the process memory was read successfully
         * @return false Otherwise
         */
        bool read_memory(base_address address, void* buffer, size_t bytes, size_t& bytes_read);

        /**
         * @brief Write data to this process' memory
         * 
         * @param address Address to which the data should be written
         * @param buffer Buffer containing the data to be written
         * @param bytes Number of bytes to write
         * @param bytes_written Number if bytes that were written
         * @return true If the process memory was written successfully
         * @return false Otherwise
         */
        bool write_memory(base_address address, void* buffer, size_t bytes, size_t& bytes_written);

        /**
         * @brief Get the current process.
         * 
         * @return ptr_type Pointer to the current process.
         */
        static ptr_type current_process();

        /**
         * @brief Find the process with the specified name.
         * 
         * @param name Name to search for
         * @return ptr_type Pointer to the located process
         */
        static ptr_type find_by_name(const name_type& name);

		/**
		 * @brief Get the name of this process.
		 * 
		 * @return Process Name
		 */
		const name_type& get_name() const;

        /**
         * @brief List running processes
         * 
         * @return list_type List of running processes.
         */
        static list_type list();

        /**
         * @brief Comparison operator. Compares with a process ID.
         * 
         * @param process_id Process ID
         * @return true If this process object's ID matches the one provided
         * @return false Otherwise
         */
        bool operator==(id_type process_id) const;

    private:

        // Process ID
        id_type m_id;
        // Process Name
        name_type m_name;
    };
}

#endif // PISTON_PROCESS_H