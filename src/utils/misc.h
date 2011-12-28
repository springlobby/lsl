#ifndef LSL_MISC_H
#define LSL_MISC_H

namespace LSL {
namespace Util {

//! delegate to boost::filesystem::exists
bool FileExists( const std::string path );
//! create temporary filestream, return is_open()
bool FileCanOpen( const std::string path );

} //namespace Util {
} //namespace LSL {

#endif // LSL_MISC_H
