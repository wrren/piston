#ifndef PISTON_PROCESS_H
#define PISTON_PROCESS_H

#include <piston/platform/types.h>
#include <string>
#include <memory>
#include <vector>

namespace piston
{
    class process
    {
    public:

        typedef piston::platform::process_id id_type;
        typedef std::string name_type;
        typedef std::shared_ptr<process> ptr_type;
        typedef std::vector<ptr_type> list_type;

        /**
         * @brief List running processes
         * 
         * @return list_type List of running processes.
         */
        static list_type list();

    private:

        id_type m_id;

        name_type m_name;
    };
}

#endif // PISTON_PROCESS_H