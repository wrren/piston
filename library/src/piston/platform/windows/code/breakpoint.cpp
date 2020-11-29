#include <piston/code/breakpoint.h>
#include <windows.h>

namespace piston
{
    bool breakpoint::is_set(register_number register_number)
    {
        CONTEXT context = { 0 };
        context.ContextFlags = CONTEXT_ALL;

        if (!GetThreadContext(GetCurrentThread(), &context))
        {
            throw breakpoint::check_exception("Failed to retrieve thread context.");
        }

        if(register_number == breakpoint::register_number::REGISTER_ANY)
        {
            return !!(context.Dr7 & 0x000000FF);
        }
        
        return !!(context.Dr7 & (1 << static_cast<uint8_t>(register_number))) || !!(context.Dr7 & (1 << (static_cast<uint8_t>(register_number) + 1)));
    }

    void breakpoint::unset(breakpoint::register_number register_number, breakpoint::scope scope)
    {
        CONTEXT context = { 0 };
        context.ContextFlags = CONTEXT_ALL;

        if (!GetThreadContext(GetCurrentThread(), &context))
        {
            throw breakpoint::check_exception("Failed to retrieve thread context.");
        }

        if(register_number == breakpoint::register_number::REGISTER_ANY)
        {
            return;
        }

        context.Dr7 &= ~(1 << (static_cast<uint8_t>(register_number) + static_cast<uint8_t>(scope)));

        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        if(!SetThreadContext(GetCurrentThread(), &context))
        {
            throw breakpoint::set_exception("Failed to set thread context.");
        }
    }

    breakpoint breakpoint::set_thread(platform::thread_id thread, handler_address handler, breakpoint_address breakpoint, size_type size, register_number register_number, break_type type)
    {
        CONTEXT context = { 0 };
        context.ContextFlags = CONTEXT_ALL;

        if (!GetThreadContext(thread, &context))
        {
            throw breakpoint::check_exception("Failed to retrieve thread context.");
        }

        breakpoint::register_number target_register = register_number;

        if(register_number == breakpoint::register_number::REGISTER_ANY)
        {
            target_register =   !is_set(breakpoint::register_number::REGISTER_DR_0) ? breakpoint::register_number::REGISTER_DR_0 :
                                !is_set(breakpoint::register_number::REGISTER_DR_1) ? breakpoint::register_number::REGISTER_DR_1 :
                                !is_set(breakpoint::register_number::REGISTER_DR_2) ? breakpoint::register_number::REGISTER_DR_2 :
                                !is_set(breakpoint::register_number::REGISTER_DR_3) ? breakpoint::register_number::REGISTER_DR_3 :
                                breakpoint::register_number::REGISTER_DR_0;
        }           

        context.Dr7 &= ~(1 << (static_cast<uint8_t>(target_register) + static_cast<uint8_t>(breakpoint::scope::BP_SCOPE_GLOBAL)));
        context.Dr7 |=  (1 << (static_cast<uint8_t>(target_register) + static_cast<uint8_t>(breakpoint::scope::BP_SCOPE_THREAD)));

        switch(target_register)
        {
            case breakpoint::register_number::REGISTER_DR_0: context.Dr0 = reinterpret_cast<DWORD64>(breakpoint); break;
            case breakpoint::register_number::REGISTER_DR_1: context.Dr1 = reinterpret_cast<DWORD64>(breakpoint); break;
            case breakpoint::register_number::REGISTER_DR_2: context.Dr2 = reinterpret_cast<DWORD64>(breakpoint); break;
            case breakpoint::register_number::REGISTER_DR_3: context.Dr3 = reinterpret_cast<DWORD64>(breakpoint); break;
        }

        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        if(!SetThreadContext(GetCurrentThread(), &context))
        {
            throw breakpoint::set_exception("Failed to set thread context.");
        }

        return breakpoint::breakpoint(handler, breakpoint, size, target_register, type, thread);
    }

    breakpoint breakpoint::set_global(handler_address handler, breakpoint_address breakpoint, size_type size, register_number register_number, break_type type)
    {
        CONTEXT context = { 0 };
        context.ContextFlags = CONTEXT_ALL;

        if (!GetThreadContext(GetCurrentThread(), &context))
        {
            throw breakpoint::check_exception("Failed to retrieve thread context.");
        }

        breakpoint::register_number target_register = register_number;

        if(register_number == breakpoint::register_number::REGISTER_ANY)
        {
            target_register =   !is_set(breakpoint::register_number::REGISTER_DR_0) ? breakpoint::register_number::REGISTER_DR_0 :
                                !is_set(breakpoint::register_number::REGISTER_DR_1) ? breakpoint::register_number::REGISTER_DR_1 :
                                !is_set(breakpoint::register_number::REGISTER_DR_2) ? breakpoint::register_number::REGISTER_DR_2 :
                                !is_set(breakpoint::register_number::REGISTER_DR_3) ? breakpoint::register_number::REGISTER_DR_3 :
                                breakpoint::register_number::REGISTER_DR_0;
        }           

        context.Dr7 &= ~(1 << (static_cast<uint8_t>(target_register) + static_cast<uint8_t>(breakpoint::scope::BP_SCOPE_THREAD)));
        context.Dr7 |=  (1 << (static_cast<uint8_t>(target_register) + static_cast<uint8_t>(breakpoint::scope::BP_SCOPE_GLOBAL)));

        switch(target_register)
        {
            case breakpoint::register_number::REGISTER_DR_0: context.Dr0 = reinterpret_cast<DWORD64>(breakpoint); break;
            case breakpoint::register_number::REGISTER_DR_1: context.Dr1 = reinterpret_cast<DWORD64>(breakpoint); break;
            case breakpoint::register_number::REGISTER_DR_2: context.Dr2 = reinterpret_cast<DWORD64>(breakpoint); break;
            case breakpoint::register_number::REGISTER_DR_3: context.Dr3 = reinterpret_cast<DWORD64>(breakpoint); break;
        }

        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        if(!SetThreadContext(GetCurrentThread(), &context))
        {
            throw breakpoint::set_exception("Failed to set thread context.");
        }

        return breakpoint::breakpoint(handler, breakpoint, size, target_register, type);
    }
}