#ifndef LIBSPRINGLOBBY_HEADERGUARD_USER_H
#define LIBSPRINGLOBBY_HEADERGUARD_USER_H

#include <utils/global_interfaces.h>
#include <utils/misc.h>
#include <string>

namespace LSL {

const unsigned int SYNC_UNKNOWN = 0;
const unsigned int SYNC_SYNCED = 1;
const unsigned int SYNC_UNSYNCED = 2;

//! @brief Struct used to store a client's status.
struct UserStatus
{
	enum RankContainer
	{
	  RANK_1,
	  RANK_2,
	  RANK_3,
	  RANK_4,
	  RANK_5,
	  RANK_6,
	  RANK_7,
	  RANK_8
	};

  bool in_game;
  bool away;
  RankContainer rank;
  bool moderator;
  bool bot;
  UserStatus(): in_game(false), away(false), rank(RANK_1), moderator(false), bot(false) {}
  std::string GetDiffString ( const UserStatus& other ) const;
};

struct UserPosition
{
	int x;
	int y;
	UserPosition(): x(-1), y(-1) {}
};

struct UserBattleStatus
{
	//!!! when adding something to this struct, also modify User::UpdateBattleStatus() !!
	// total 17 members here
	int team;
	int ally;
	lslColor color;
	int color_index;
	int handicap;
	int side;
	unsigned int sync;
	bool spectator;
	bool ready;
	bool isfromdemo;
	UserPosition pos; // for startpos = 4
	// bot-only stuff
	std::string owner;
	std::string aishortname;
	std::string airawname;
	std::string aiversion;
	int aitype;
	// for nat holepunching
	std::string ip;
	unsigned int udpport;
	std::string scriptPassword;
	bool IsBot() const { return !aishortname.empty(); }
	UserBattleStatus(): team(0),ally(0),color(lslColor(0,0,0)),color_index(-1),handicap(0),
		side(0),sync(SYNC_UNKNOWN),spectator(false),ready(false), isfromdemo(false), aitype(-1), udpport(0)
	{}

	bool operator == ( const UserBattleStatus& s ) const
	{
		return ( ( team == s.team ) && ( color == s.color ) && ( handicap == s.handicap ) && ( side == s.side )
				 && ( sync == s.sync ) && ( spectator == s.spectator ) && ( ready == s.ready )
				 && ( owner == s.owner ) && ( aishortname == s.aishortname ) && ( isfromdemo == s.isfromdemo )
				 && ( aitype == s.aitype ) );
	}

	bool operator != ( const UserBattleStatus& s ) const
	{
//		return ( ( team != s.team ) || ( color != s.color ) || ( handicap != s.handicap ) || ( side != s.side )
//				 || ( sync != s.sync ) || ( spectator != s.spectator ) || ( ready != s.ready )
//				 || ( owner != s.owner ) || ( aishortname != s.aishortname ) || ( isfromdemo != s.isfromdemo )
//				 || ( aitype != s.aitype ) );
		return !(this->operator ==(s));
	}
};


class User : public HasIndex< std::string >
{
public:
    User() {}

    std::string index() const {return "dummy";}
    static std::string className() { return "Channel"; }
	std::string Nick() const { return "Nick"; }
};

} // namespace LSL

#endif // LIBSPRINGLOBBY_HEADERGUARD_USER_H
