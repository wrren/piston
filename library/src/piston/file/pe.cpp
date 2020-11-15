#include <piston/file/pe.h>

namespace piston
{
    pe_file::pe_file()
    {}

    pe_file::pe_file(const path& path) :
    file(path)
    {}
}