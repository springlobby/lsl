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
    ReturnType r = 0;
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
	return ToString((int)FromString<long>( hash ) );
}

// convert const char* to std::string, as std::string(NULL) crashes
static inline std::string SafeString(const char* str)
{
	if (str == NULL) return "";
	return std::string(str);
}

// convert std::string to std::wstring

#ifdef WIN32
std::wstring s2ws(const std::string& s);
std::string ws2s(const std::wstring& s);
std::string geterrormsg();
#endif

} // namespace Util
} // namespace LSL

#endif // LSL_CONVERSION_H
