#ifndef LSL_CONVERSION_H
#define LSL_CONVERSION_H

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
static inline std::string ToString(T arg){
	std::stringstream s;
	s << arg;
	return s.str();
}

} // namespace Util
} // namespace LSL

#endif // LSL_CONVERSION_H
