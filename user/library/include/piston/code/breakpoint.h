#ifndef PISTON_CODE_BREAKPOINT_H
#define PISTON_CODE_BREAKPOINT_H

#include <vector>
#include <mutex>
#include <piston/core/core.h>

namespace Piston
{
    class Breakpoint
    {
    public:

        /**
         * @brief Exception type thrown when a breakpoint or register cannot be checked.
         * 
         */
        class CheckException : public std::exception
        {
        public:

            /**
             * @brief Construct a new check exception object
             * 
             * @param message Error message
             */
            CheckException(const char* message);

            /**
             * @brief Return the human-readable message associated with this exception.
             * 
             * @return const char* 
             */
            virtual const char* what() const noexcept override;

        private:

            // Error Message
            String mMessage;
        };

        /**
         * @brief Exception type thrown when a breakpoint or register cannot be set.
         * 
         */
        class SetException : public std::exception
        {
        public:

            /**
             * @brief Construct a new set exception object
             * 
             * @param message Error message
             */
            SetException(const char* message);

            /**
             * @brief Return the human-readable message associated with this exception.
             * 
             * @return const char* 
             */
            virtual const char* what() const noexcept override;

        private:

            // Error Message
            String mMessage;
        };

        // Address of a breakpoint handler
        typedef void* HandlerAddress;
        // Address of the code or memory to break on
        typedef void* BreakpointAddress;
        // Breakpoint list type
        typedef std::vector<Breakpoint> ListType;
        // Breakpoint size
        typedef uint8_t SizeType;

        enum class Register : uint8_t
        {
            REGISTER_ANY,
            REGISTER_DR_0 = 0,
            REGISTER_DR_1 = 2,
            REGISTER_DR_2 = 4,
            REGISTER_DR_3 = 6
        };

        enum class Scope
        {
            BP_SCOPE_THREAD = 0,
            BP_SCOPE_GLOBAL = 1
        };

        enum class BreakType
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
        static bool IsSet(Register reg);

        /**
         * @brief Enable the hardware breakpoint set on the given register
         * 
         * @param reg Register Number
         * @param scope Breakpoint scope
         */
        static void Set(Register reg, Scope scope);

        /**
         * @brief Disable the hardware breakpoint set on the given register
         * 
         * @param reg Register Number
         * @param scope Breakpoint scope
         */
        static void Unset(Register reg, Scope scope);

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
        static Breakpoint SetThreadBreakpoint(
            Platform::ThreadID thread, 
            HandlerAddress handler, 
            BreakpointAddress breakpoint, 
            SizeType size, 
            Register register_number = Register::REGISTER_ANY,
            BreakType type = BreakType::BP_TYPE_CODE);

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
        static Breakpoint SetGlobalBreakpoint(
            HandlerAddress handler, 
            BreakpointAddress breakpoint, 
            SizeType size, 
            Register register_number = Register::REGISTER_ANY,
            BreakType type = BreakType::BP_TYPE_CODE);

        /**
         * @brief Compare one breakpoint object to another
         * 
         * @param breakpoint Breakpoint against which to compare this object
         * @return true If this is equivalent to the given breakpoint
         * @return false False otherwise
         */
        bool operator==(const Breakpoint& breakpoint) const;

        /**
         * @brief Indicates that a breakpoint has been hit, removes it from the global breakpoint list.
         * 
         */
        void WasHit();

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
        Breakpoint(HandlerAddress handler, BreakpointAddress breakpoint, SizeType size, Register register_number, BreakType type);

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
        Breakpoint(HandlerAddress handler, BreakpointAddress breakpoint, SizeType size, Register register_number, BreakType type, Platform::ThreadID thread_id);

        // Breakpoint Handler Address
        HandlerAddress mHandler;
        // Address of the code or memory to break on
        BreakpointAddress mAddress;
        // Breakpoint register used to set this breakpoint
        Register mRegister; 
        // Breakpoint type
        BreakType mType;
        // Breakpoint size, in bytes
        SizeType mSize;
        // Indicates the thread on which the breakpoint was set
        Platform::ThreadID mThread;
        // Breakpoint scope
        Scope mScope;
        // Global breakpoint list
        static ListType gBreakpoints;
        // Breakpoint list mutex
        static std::mutex gMutex;
    };
}

#endif // PISTON_CODE_BREAKPOINT_H