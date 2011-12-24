#ifndef LSL_DATATYPES_H
#define LSL_DATATYPES_H
namespace LSL {
class User;

typedef std::map< std::string, std::string> StringMap;
typedef std::vector< std::string > StringVector;
typedef std::vector< User* > UserVector;
}//namespace LSL
#endif //LSL_DATATYPES_H
