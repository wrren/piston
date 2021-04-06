#ifndef PISTON_PROCESS_H
#define PISTON_PROCESS_H

#include <vector>

namespace Piston
{
    class Process
    {
    public:

        /**
         * @brief Listener interface for global process events.
         * 
         */
        class listener
        {
        public:

            /**
             * @brief Called when a new process is created
             * 
             * @param process The created process
             */
            virtual void on_create(const Process& process);

            /**
             * @brief Called when a running process ends and is deleted
             * 
             * @param process The deleted process
             */
            virtual void on_delete(const Process& process);
        };

        /**
         * @brief Add a process event listener
         * 
         * @param listener Listener
         */
        static void add_listener(listener* listener);

        /**
         * @brief Remove a process event listener
         * 
         * @param listener Listener
         */
        static void remove_listener(listener* listener);

    private:

        // Global process event listeners
        static std::vector<listener*> g_listeners;
    };
}

#endif // PISTON_PROCESS_H