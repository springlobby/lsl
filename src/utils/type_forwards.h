#ifndef LSL_DATATYPES_H
#define LSL_DATATYPES_H

namespace boost {
template < class T >
class shared_ptr;
}

namespace LSL {
namespace Battle {
	class Battle;
}

class User;
class Channel;
class Server;

//typedef std::map< std::string, std::string> StringMap;
//typedef std::vector< std::string > StringVector;
//typedef std::vector< UserPtr > UserVector;

typedef boost::shared_ptr< User > UserPtr;
typedef boost::shared_ptr< const User > ConstUserPtr;

typedef boost::shared_ptr< Battle::Battle > BattlePtr;
typedef boost::shared_ptr< const Battle::Battle > ConstBattlePtr;

typedef boost::shared_ptr< Channel > ChannelPtr;
typedef boost::shared_ptr< const Channel > ConstChannelPtr;

typedef boost::shared_ptr< Server > ServerPtr;
typedef boost::shared_ptr< const Server > ConstServerPtr;

}//namespace LSL
#endif //LSL_DATATYPES_H
