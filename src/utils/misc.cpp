#include "misc.h"

#include <boost/filesystem.hpp>
#include <fstream>

namespace LSL {
namespace Util {

bool FileExists( const std::string path )
{
    return boost::filesystem::exists( boost::filesystem::path( path ) );
}

bool FileCanOpen( const std::string path )
{
    return std::ifstream(path).is_open();
}

} //namespace Util
} //namespace LSL
