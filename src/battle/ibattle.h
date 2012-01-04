#ifndef LIBSPRINGLOBBY_HEADERGUARD_IBATTLE_H
#define LIBSPRINGLOBBY_HEADERGUARD_IBATTLE_H

#include <utils/global_interfaces.h>
#include "enum.h"

namespace LSL {
namespace Battle {

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
	BattleOptions();

	int battleid;
	bool islocked;
	Enum::BattleType battletype;
	bool ispassworded;
	int rankneeded;
	std::string proxyhost;
	bool userelayhost;
	bool lockexternalbalancechanges;

	std::string founder;

	Enum::NatType nattype;
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
	static std::string className() { return "IBattle"; }

	bool Ingame() const { return false; }
	Enum::NatType GetNatType() const { return Enum::NAT_None; }
	bool IsFounderMe() const { return false; }
	bool IsProxy() const { return false; }

};

} // namespace Battle
} // namespace LSL

#endif // LIBSPRINGLOBBY_HEADERGUARD_IBATTLE_H
