/* This file is part of the Springlobby (GPL v2 or later), see COPYING */

#ifndef LSL_CONVERSION_H
#define LSL_CONVERSION_H

#include <sstream>

namespace LSL {
namespace Util {

template < class ReturnType, class T >
ReturnType FromString(const T s)
{
    std::stringstream ss;
    ss << s;
    ReturnType r;
    ss >> r;
    return r;
}

template<class T>
static inline std::string ToString(const T arg){
	std::stringstream s;
	s << arg;
	return s.str();
}

static inline std::string MakeHashUnsigned( const std::string& hash )
{
	return ToString( FromString<unsigned int>( hash ) );
}

static inline std::string MakeHashSigned( const std::string& hash )
{
	return ToString( FromString<int>( hash ) );
}
// convert std::string to std::wstring

#ifdef WIN32
std::wstring s2ws(const std::string& s);
std::string geterrormsg();
#endif

} // namespace Util
} // namespace LSL

#endif // LSL_CONVERSION_H
