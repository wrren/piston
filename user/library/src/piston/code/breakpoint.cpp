#include <piston/code/breakpoint.h>
#include <algorithm>

namespace Piston
{
    Breakpoint::ListType Breakpoint::gBreakpoints;
    std::mutex Breakpoint::gMutex;

    Breakpoint::CheckException::CheckException(const char* message) :
    mMessage(message)
    {}

    const char* Breakpoint::CheckException::what() const noexcept
    {
        return mMessage.c_str();
    }

    Breakpoint::SetException::SetException(const char* message) :
    mMessage(message)
    {}

    const char* Breakpoint::SetException::what() const noexcept
    {
        return mMessage.c_str();
    }

    Breakpoint::Breakpoint(HandlerAddress handler, BreakpointAddress breakpoint, SizeType size, Register register_number, BreakType type) :
    mHandler(handler),
    mAddress(breakpoint),
    mSize(size),
    mRegister(register_number),
    mType(type),
    mScope(Breakpoint::Scope::BP_SCOPE_GLOBAL)
    {
        std::lock_guard<std::mutex> guard(gMutex);
        gBreakpoints.push_back(*this);
    }

    Breakpoint::Breakpoint(HandlerAddress handler, BreakpointAddress breakpoint, SizeType size, Register register_number, BreakType type, Platform::ThreadID thread_id) :
    mThread(thread_id),
    mHandler(handler),
    mAddress(breakpoint),
    mSize(size),
    mRegister(register_number),
    mType(type),
    mScope(Breakpoint::Scope::BP_SCOPE_THREAD)
    {
        std::lock_guard<std::mutex> guard(gMutex);
        gBreakpoints.push_back(*this);
    }

    void Breakpoint::WasHit()
    {
        std::lock_guard<std::mutex> guard(gMutex);
        gBreakpoints.erase(std::remove(gBreakpoints.begin(), gBreakpoints.end(), *this), gBreakpoints.end());
        Unset(mRegister, Breakpoint::Scope::BP_SCOPE_GLOBAL);
        Unset(mRegister, Breakpoint::Scope::BP_SCOPE_THREAD);
    }

    bool Breakpoint::operator==(const Breakpoint& other) const
    {
        return  mScope         == other.mScope   &&
                mHandler       == other.mHandler      &&
                mAddress    == other.mAddress   &&
                mRegister      == other.mRegister     &&
                mSize          == other.mSize;
    }
}