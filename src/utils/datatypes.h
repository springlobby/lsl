#ifndef LSL_DATATYPES_H
#define LSL_DATATYPES_H

#include <map>
#include <string>
#include <vector>
//#include <boost/shared_ptr.hpp>

namespace LSL {
class User;

typedef std::map< std::string, std::string> StringMap;
typedef std::vector< std::string > StringVector;
//typedef std::vector< User* > UserVector;
}//namespace LSL
#endif //LSL_DATATYPES_H
