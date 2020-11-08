#ifndef PISTON_PROCESS_H
#define PISTON_PROCESS_H

#include <piston/core/core.h>
#include <memory>
#include <vector>

namespace piston
{
    class process
    {
    public:

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

            virtual const char* what() const override;
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

        id_type m_id;

        name_type m_name;
    };
}

#endif // PISTON_PROCESS_H