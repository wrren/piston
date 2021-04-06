#ifndef PISTON_CORE_TYPES_H
#define PISTON_CORE_TYPES_H

#include <memory>
#include <string>
#include <sstream>
#include <filesystem>
#include <cstdint>
#include <exception>
#include <deque>
#include <vector>
#include <map>
#include <functional>

namespace Piston
{
  typedef std::string String;
  typedef std::wstring WideString;
  typedef std::stringstream StringStream;
  typedef std::wstringstream WideStringStream;
  typedef std::filesystem::path Path;
  typedef uint8_t byte;
}

#endif // PISTON_CORE_TYPES_H