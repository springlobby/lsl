#ifndef LSL_DATATYPES_H
#define LSL_DATATYPES_H

//there's no good way to forward these
#include <string>
#include <map>
#include <vector>

namespace boost {
template < class T >
class shared_ptr;
}

namespace std {

}

namespace LSL {
namespace Battle {
	class IBattle;
	class Battle;
	class BattleOptions;
}

class User;
class Channel;
class Server;
class UnitsyncMap;
class UnitsyncMod;
class UserBattleStatus;
class UserPosition;
class OptionsWrapper;
struct GameOptions;

typedef std::map< std::string, std::string> StringMap;
typedef std::vector< std::string > StringVector;

typedef boost::shared_ptr< User > UserPtr;
typedef boost::shared_ptr< const User > ConstUserPtr;

typedef boost::shared_ptr< Battle::IBattle > IBattlePtr;
typedef boost::shared_ptr< const Battle::IBattle > ConstIBattlePtr;

typedef boost::shared_ptr< Battle::Battle > BattlePtr;
typedef boost::shared_ptr< const Battle::Battle > ConstBattlePtr;

typedef boost::shared_ptr< Channel > ChannelPtr;
typedef boost::shared_ptr< const Channel > ConstChannelPtr;

typedef boost::shared_ptr< Server > ServerPtr;
typedef boost::shared_ptr< const Server > ConstServerPtr;

typedef std::vector< UserPtr > UserVector;
typedef std::vector< ConstUserPtr > ConstUserVector;

typedef boost::shared_ptr< OptionsWrapper > OptionsWrapperPtr;
typedef boost::shared_ptr< const OptionsWrapper > ConstOptionsWrapperPtr;


class mmOptionBool;
class mmOptionFloat;
class mmOptionString;
class mmOptionList;
class mmOptionSection;

typedef std::map<std::string,mmOptionBool> OptionMapBool;
typedef std::map<std::string,mmOptionFloat> OptionMapFloat;
typedef std::map<std::string,mmOptionString> OptionMapString;
typedef std::map<std::string,mmOptionList> OptionMapList;
typedef std::map<std::string,mmOptionSection> OptionMapSection;

typedef std::map<std::string,mmOptionBool>::iterator OptionMapBoolIter;
typedef std::map<std::string,mmOptionFloat>::iterator OptionMapFloatIter;
typedef std::map<std::string,mmOptionString>::iterator OptionMapStringIter;
typedef std::map<std::string,mmOptionList>::iterator OptionMapListIter;
typedef std::map<std::string,mmOptionSection>::iterator OptionMapSectionIter;

typedef std::map<std::string,mmOptionBool>::const_iterator OptionMapBoolConstIter;
typedef std::map<std::string,mmOptionFloat>::const_iterator OptionMapFloatConstIter;
typedef std::map<std::string,mmOptionString>::const_iterator OptionMapStringConstIter;
typedef std::map<std::string,mmOptionList>::const_iterator OptionMapListConstIter;
typedef std::map<std::string,mmOptionSection>::const_iterator OptionMapSectionConstIter;

}//namespace LSL
#endif //LSL_DATATYPES_H
