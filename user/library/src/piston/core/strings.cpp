#include <piston/core/strings.h>
#include <exception>

namespace Piston
{
    union SingleCharacter
    {
        wchar_t WideChar;
        struct
        {
            char UpperChar;
            char LowerChar;
        } Components;
    };

    WideString Strings::StringToWideString(const String& Input)
    {
        return WideString(Input.begin(), Input.end());
    }

    String Strings::WideStringToString(const WideString& Input)
    {
        String Output;
        Output.reserve(Input.length());

        for(size_t i = 0; i < Input.length(); i++)
        {
            SingleCharacter Character;
            Character.WideChar = Input[i];

            if(Character.Components.LowerChar == 0)
            {
                Output.push_back(Character.Components.UpperChar);
            }
            else
            {
                throw std::invalid_argument("Input string does not contain exclusively ASCII characters.");
            }
        }

        return Output;
    }
}