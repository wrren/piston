#ifndef PISTON_CORE_TYPES_H
#define PISTON_CORE_TYPES_H

#include <memory>
#include <string>
#include <sstream>
#include <filesystem>
#include <cstdint>
#include <exception>

namespace piston
{
  typedef std::string string;
  typedef std::wstring wstring;
  typedef std::stringstream stringstream;
  typedef std::wstringstream wstringstream;
  typedef std::filesystem::path path;
}

#endif // PISTON_CORE_TYPES_H