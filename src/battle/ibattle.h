#ifndef LIBSPRINGLOBBY_HEADERGUARD_IBATTLE_H
#define LIBSPRINGLOBBY_HEADERGUARD_IBATTLE_H

#include <utils/global_interfaces.h>

namespace LSL {
namespace Battle {

enum NatType
{
	NAT_None = 0,
	NAT_Hole_punching,
	NAT_Fixed_source_ports
};


enum BattleType
{
	BT_Played,
	BT_Replay,
	BT_Savegame
};

enum BalanceType
{
	balance_divide,
	balance_random
};

enum StartType
{
	ST_Fixed = 0,
	ST_Random = 1,
	ST_Choose = 2,
	ST_Pick = 3
};

enum GameType
{
	GT_ComContinue = 0,
	GT_ComEnds = 1,
	GT_Lineage = 2
};

const unsigned int DEFAULT_SERVER_PORT = 8452;
const unsigned int DEFAULT_EXTERNAL_UDP_SOURCE_PORT = 16941;

class IBattle;

struct BattleStartRect
{
	BattleStartRect() :
		toadd(false),
		todelete(false),
		toresize(false),
		exist(false),
		ally(-1),
		top(-1),
		left(-1),
		right(-1),
		bottom(-1)
	{
	}

	bool toadd;
	bool todelete;
	bool toresize;
	bool exist;

	int ally;
	int top;
	int left;
	int right;
	int bottom;

	bool IsOk() const
	{
		return exist && !todelete;
	}
};

struct BattleOptions
{
	BattleOptions() :
		battleid(-1),islocked(false),battletype(BT_Played),ispassworded(false),
		rankneeded(0),proxyhost(""),userelayhost(false),
		lockexternalbalancechanges(false),nattype(NAT_None),port(DEFAULT_SERVER_PORT),
		externaludpsourceport(DEFAULT_EXTERNAL_UDP_SOURCE_PORT),
		internaludpsourceport(DEFAULT_EXTERNAL_UDP_SOURCE_PORT),
		maxplayers(0),spectators(0)
	{}

	int battleid;
	bool islocked;
	BattleType battletype;
	bool ispassworded;
	int rankneeded;
	std::string proxyhost;
	bool userelayhost;
	bool lockexternalbalancechanges;

	std::string founder;

	NatType nattype;
	unsigned int port;
	std::string ip;
	unsigned int externaludpsourceport;
	unsigned int internaludpsourceport;

	unsigned int maxplayers;
	unsigned int spectators;
	std::string relayhost;
	std::string maphash;
	std::string modhash;

	std::string description;
	std::string mapname;
	std::string modname;
};

class IBattle : public HasIndex< int >
{
public:
    IBattle() {}

    int index() const { return 1; }
    static std::string className() { return "Channel"; }


};

} // namespace Battle
} // namespace LSL

#endif // LIBSPRINGLOBBY_HEADERGUARD_IBATTLE_H
