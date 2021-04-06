#include <piston/code/breakpoint.h>
#include <windows.h>

namespace Piston
{
    bool Breakpoint::IsSet(Register register_number)
    {
        CONTEXT context = { 0 };
        context.ContextFlags = CONTEXT_ALL;

        if (!GetThreadContext(GetCurrentThread(), &context))
        {
            throw Breakpoint::CheckException("Failed to retrieve thread context.");
        }

        if(register_number == Breakpoint::Register::REGISTER_ANY)
        {
            return !!(context.Dr7 & 0x000000FF);
        }
        
        return !!(context.Dr7 & (1 << static_cast<uint8_t>(register_number))) || !!(context.Dr7 & (1 << (static_cast<uint8_t>(register_number) + 1)));
    }

    void Breakpoint::Unset(Breakpoint::Register register_number, Breakpoint::Scope scope)
    {
        CONTEXT context = { 0 };
        context.ContextFlags = CONTEXT_ALL;

        if (!GetThreadContext(GetCurrentThread(), &context))
        {
            throw Breakpoint::CheckException("Failed to retrieve thread context.");
        }

        if(register_number == Breakpoint::Register::REGISTER_ANY)
        {
            return;
        }

        context.Dr7 &= ~(1 << (static_cast<uint8_t>(register_number) + static_cast<uint8_t>(scope)));

        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        if(!SetThreadContext(GetCurrentThread(), &context))
        {
            throw Breakpoint::SetException("Failed to set thread context.");
        }
    }

    Breakpoint Breakpoint::SetThreadBreakpoint(Platform::ThreadID thread, HandlerAddress handler, BreakpointAddress breakpoint, SizeType size, Register register_number, BreakType type)
    {
        CONTEXT context = { 0 };
        context.ContextFlags = CONTEXT_ALL;

        if (!GetThreadContext(thread, &context))
        {
            throw Breakpoint::CheckException("Failed to retrieve thread context.");
        }

        Breakpoint::Register target_register = register_number;

        if(register_number == Breakpoint::Register::REGISTER_ANY)
        {
            target_register =   !IsSet(Breakpoint::Register::REGISTER_DR_0) ? Breakpoint::Register::REGISTER_DR_0 :
                                !IsSet(Breakpoint::Register::REGISTER_DR_1) ? Breakpoint::Register::REGISTER_DR_1 :
                                !IsSet(Breakpoint::Register::REGISTER_DR_2) ? Breakpoint::Register::REGISTER_DR_2 :
                                !IsSet(Breakpoint::Register::REGISTER_DR_3) ? Breakpoint::Register::REGISTER_DR_3 :
                                Breakpoint::Register::REGISTER_DR_0;
        }           

        context.Dr7 &= ~(1 << (static_cast<uint8_t>(target_register) + static_cast<uint8_t>(Breakpoint::Scope::BP_SCOPE_GLOBAL)));
        context.Dr7 |=  (1 << (static_cast<uint8_t>(target_register) + static_cast<uint8_t>(Breakpoint::Scope::BP_SCOPE_THREAD)));

        switch(target_register)
        {
            case Breakpoint::Register::REGISTER_DR_0: context.Dr0 = reinterpret_cast<DWORD64>(breakpoint); break;
            case Breakpoint::Register::REGISTER_DR_1: context.Dr1 = reinterpret_cast<DWORD64>(breakpoint); break;
            case Breakpoint::Register::REGISTER_DR_2: context.Dr2 = reinterpret_cast<DWORD64>(breakpoint); break;
            case Breakpoint::Register::REGISTER_DR_3: context.Dr3 = reinterpret_cast<DWORD64>(breakpoint); break;
        }

        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        if(!SetThreadContext(GetCurrentThread(), &context))
        {
            throw Breakpoint::SetException("Failed to set thread context.");
        }

        return Breakpoint::Breakpoint(handler, breakpoint, size, target_register, type, thread);
    }

    Breakpoint Breakpoint::SetGlobalBreakpoint(HandlerAddress handler, BreakpointAddress breakpoint, SizeType size, Register register_number, BreakType type)
    {
        CONTEXT context = { 0 };
        context.ContextFlags = CONTEXT_ALL;

        if (!GetThreadContext(GetCurrentThread(), &context))
        {
            throw Breakpoint::CheckException("Failed to retrieve thread context.");
        }

        Breakpoint::Register target_register = register_number;

        if(register_number == Breakpoint::Register::REGISTER_ANY)
        {
            target_register =   !IsSet(Breakpoint::Register::REGISTER_DR_0) ? Breakpoint::Register::REGISTER_DR_0 :
                                !IsSet(Breakpoint::Register::REGISTER_DR_1) ? Breakpoint::Register::REGISTER_DR_1 :
                                !IsSet(Breakpoint::Register::REGISTER_DR_2) ? Breakpoint::Register::REGISTER_DR_2 :
                                !IsSet(Breakpoint::Register::REGISTER_DR_3) ? Breakpoint::Register::REGISTER_DR_3 :
                                Breakpoint::Register::REGISTER_DR_0;
        }           

        context.Dr7 &= ~(1 << (static_cast<uint8_t>(target_register) + static_cast<uint8_t>(Breakpoint::Scope::BP_SCOPE_THREAD)));
        context.Dr7 |=  (1 << (static_cast<uint8_t>(target_register) + static_cast<uint8_t>(Breakpoint::Scope::BP_SCOPE_GLOBAL)));

        switch(target_register)
        {
            case Breakpoint::Register::REGISTER_DR_0: context.Dr0 = reinterpret_cast<DWORD64>(breakpoint); break;
            case Breakpoint::Register::REGISTER_DR_1: context.Dr1 = reinterpret_cast<DWORD64>(breakpoint); break;
            case Breakpoint::Register::REGISTER_DR_2: context.Dr2 = reinterpret_cast<DWORD64>(breakpoint); break;
            case Breakpoint::Register::REGISTER_DR_3: context.Dr3 = reinterpret_cast<DWORD64>(breakpoint); break;
        }

        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        if(!SetThreadContext(GetCurrentThread(), &context))
        {
            throw Breakpoint::SetException("Failed to set thread context.");
        }

        return Breakpoint::Breakpoint(handler, breakpoint, size, target_register, type);
    }
}