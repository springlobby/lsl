#ifndef LSL_CONVERSION_H
#define LSL_CONVERSION_H

namespace LSL {

template < class ReturnType, class T >
ReturnType FromString(const T s)
{
    std::stringstream ss;
    ss << s;
    ReturnType r;
    ss >> r;
    return r;
}

} // namespace LSL

#endif // LSL_CONVERSION_H
