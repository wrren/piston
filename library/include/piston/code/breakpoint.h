#ifndef PISTON_CODE_BREAKPOINT_H
#define PISTON_CODE_BREAKPOINT_H

#include <vector>
#include <mutex>
#include <piston/core/core.h>

namespace piston
{
    class breakpoint
    {
    public:

        /**
         * @brief Exception type thrown when a breakpoint or register cannot be checked.
         * 
         */
        class check_exception : public std::exception
        {
        public:

            /**
             * @brief Construct a new check exception object
             * 
             * @param message Error message
             */
            check_exception(const char* message);

            /**
             * @brief Return the human-readable message associated with this exception.
             * 
             * @return const char* 
             */
            virtual const char* what() const override;

        private:

            // Error Message
            string m_message;
        };

        /**
         * @brief Exception type thrown when a breakpoint or register cannot be set.
         * 
         */
        class set_exception : public std::exception
        {
        public:

            /**
             * @brief Construct a new set exception object
             * 
             * @param message Error message
             */
            set_exception(const char* message);

            /**
             * @brief Return the human-readable message associated with this exception.
             * 
             * @return const char* 
             */
            virtual const char* what() const override;

        private:

            // Error Message
            string m_message;
        };

        // Address of a breakpoint handler
        typedef void* handler_address;
        // Address of the code or memory to break on
        typedef void* breakpoint_address;
        // Breakpoint list type
        typedef std::vector<breakpoint> list;
        // Breakpoint size
        typedef uint8_t size_type;

        enum class register_number : uint8_t
        {
            REGISTER_ANY,
            REGISTER_DR_0 = 0,
            REGISTER_DR_1 = 2,
            REGISTER_DR_2 = 4,
            REGISTER_DR_3 = 6
        };

        enum class scope
        {
            BP_SCOPE_THREAD = 0,
            BP_SCOPE_GLOBAL = 1
        };

        enum class break_type
        {
            BP_TYPE_CODE,
            BP_TYPE_WRITE,
            BP_TYPE_READWRITE
        };

        /**
         * @brief Determine whether the given breakpoint register has been set
         * 
         * @param reg Register to check
         * @return true If the given register has a breakpoint set
         * @return false Otherwise
         */
        static bool is_set(register_number reg);

        /**
         * @brief Enable the hardware breakpoint set on the given register
         * 
         * @param reg Register Number
         * @param scope Breakpoint scope
         */
        static void set(register_number reg, scope scope);

        /**
         * @brief Disable the hardware breakpoint set on the given register
         * 
         * @param reg Register Number
         * @param scope Breakpoint scope
         */
        static void unset(register_number reg, scope scope);

        /**
         * @brief Set a new thread-local breakpoint
         * 
         * @param thread Thread on which to set the breakpoint
         * @param handler Handler function for the breakpoint
         * @param breakpoint Address on which to set the breakpoint
         * @param size Breakpoint size, in bytes
         * @param register_number Breakpoint register to be used
         * @param type Breakpoint type
         * @return breakpoint 
         */
        static breakpoint set_thread(
            platform::thread_id thread, 
            handler_address handler, 
            breakpoint_address breakpoint, 
            size_type size, 
            register_number register_number = register_number::REGISTER_ANY,
            break_type type = break_type::BP_TYPE_CODE);

        /**
         * @brief Set a new global breakpoint
         * 
         * @param handler Handler function for the breakpoint
         * @param breakpoint Address on which to set the breakpoint
         * @param size Breakpoint size, in bytes
         * @param register_number Breakpoint register to be used
         * @param type Breakpoint type
         * @return breakpoint 
         */
        static breakpoint set_global(
            handler_address handler, 
            breakpoint_address breakpoint, 
            size_type size, 
            register_number register_number = register_number::REGISTER_ANY,
            break_type type = break_type::BP_TYPE_CODE);

        /**
         * @brief Compare one breakpoint object to another
         * 
         * @param breakpoint Breakpoint against which to compare this object
         * @return true If this is equivalent to the given breakpoint
         * @return false False otherwise
         */
        bool operator==(const breakpoint& breakpoint) const;

        /**
         * @brief Indicates that a breakpoint has been hit, removes it from the global breakpoint list.
         * 
         */
        void hit();

    private:

        /**
         * @brief Construct a new global breakpoint
         * 
         * @param handler Address of the handler for this breakpoint
         * @param breakpoint Address of the code or memory to break on
         * @param size Size of the breakpoint
         * @param register_number Register used to set this breakpoint
         * @param type Breakpoint type
         */
        breakpoint(handler_address handler, breakpoint_address breakpoint, size_type size, register_number register_number, break_type type);

        /**
         * @brief Construct a new thread-local breakpoint
         * 
         * @param handler Address of the handler for this breakpoint
         * @param breakpoint Address of the code or memory to break on
         * @param size Size of the breakpoint
         * @param register_number Register used to set this breakpoint
         * @param type Breakpoint type
         * @param thread_id Thread on which the breakpoint is set
         */
        breakpoint(handler_address handler, breakpoint_address breakpoint, size_type size, register_number register_number, break_type type, platform::thread_id thread_id);

        // Breakpoint Handler Address
        handler_address m_handler;
        // Address of the code or memory to break on
        breakpoint_address m_address;
        // Breakpoint register used to set this breakpoint
        register_number m_register; 
        // Breakpoint type
        break_type m_type;
        // Breakpoint size, in bytes
        size_type m_size;
        // Indicates the thread on which the breakpoint was set
        platform::thread_id m_thread;
        // Breakpoint scope
        scope m_scope;
        // Global breakpoint list
        static list g_breakpoints;
        // Breakpoint list mutex
        static std::mutex g_mutex;
    };
}

#endif // PISTON_CODE_BREAKPOINT_H