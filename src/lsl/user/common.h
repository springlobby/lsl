#ifndef LIBSPRINGLOBBY_HEADERGUARD_COMMON_USER_H
#define LIBSPRINGLOBBY_HEADERGUARD_COMMON_USER_H

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
	UserBattleStatus();

	bool operator == ( const UserBattleStatus& s ) const;
	bool operator != ( const UserBattleStatus& s ) const;
};

//! parent class leaving out server related functionality
class CommonUser : public HasKey< std::string >
{
public:
	CommonUser(const std::string& nick, const std::string& country, const int& cpu)
		: m_nick(std::string(nick)), m_country(std::string(country)), m_cpu(cpu)  {}

	virtual ~CommonUser(){}

	std::string key() const {return m_id;}
	static std::string className() { return "Channel"; }

	const std::string& Nick() const { return m_nick; }
	virtual void SetNick( const std::string& nick ) { m_nick = nick; }

	const std::string& GetCountry() const { return m_country; }
	virtual void SetCountry( const std::string& country ) { m_country = country; }

	int GetCpu() const { return m_cpu; }
	void SetCpu( const int& cpu ) { m_cpu = cpu; }

	const std::string& GetID() const { return m_id; }
	void SetID( const std::string& id ) { m_id = id; }

	UserStatus& Status() { return m_status; }

	UserStatus GetStatus() const { return m_status; }
	virtual void SetStatus( const UserStatus& status );

	UserBattleStatus& BattleStatus() { return m_bstatus; }

	UserBattleStatus GetBattleStatus() const { return m_bstatus; }

	/** Read-only variant of BattleStatus() above.
	 */
	const UserBattleStatus& BattleStatus() const {
		return m_bstatus;
	}

	//void SetBattleStatus( const UserBattleStatus& status );/// dont use this to avoid overwriting data like ip and port, use following method.
	void UpdateBattleStatus( const UserBattleStatus& status );

	bool Equals( const CommonUser& other ) const { return ( m_nick == other.Nick() ); }


protected:
	std::string m_nick;
	std::string m_country;
	std::string m_id;
	int m_cpu;
	UserStatus m_status;
	UserBattleStatus m_bstatus;
};

} // namespace LSL

#endif // LIBSPRINGLOBBY_HEADERGUARD_COMMON_USER_H
