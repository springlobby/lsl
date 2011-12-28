#include "misc.h"

#include <boost/filesystem.hpp>
#include <fstream>

bool LSL::FileExists( const std::string path )
{
    return boost::filesystem::exists( boost::filesystem::path( path ) );
}

bool LSL::FileCanOpen( const std::string path )
{
    return std::ifstream(path).is_open();
}
