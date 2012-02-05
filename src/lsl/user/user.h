#ifndef LSL_USER_H
#define LSL_USER_H

/** \file battlelist.h
    \copyright GPL v2 **/

#include "common.h"

#include <lslutils/type_forwards.h>
#include <boost/shared_ptr.hpp>


namespace LSL {

class iServer;

//! actual "Online user"
class User : public CommonUser
{
public:
	User( IServerPtr serv );
	User( const std::string& nick, IServerPtr serv );
    User( const std::string& nick, const std::string& country, const int cpu, IServerPtr serv);
	User( const std::string& nick );
    User( const std::string& nick, const std::string& country, const int cpu );
	User();

	virtual ~User();

	// User interface

	ConstIServerPtr GetServer() const { return m_serv; }

	void Said( const std::string& message ) const;
	void Say( const std::string& message ) const;
	void DoAction( const std::string& message ) const;

	BattlePtr GetBattle() const;
	void SetBattle( BattlePtr battle );

	void SendMyUserStatus() const;
	void SetStatus( const UserStatus& status );
	void SetCountry( const std::string& country );

	bool ExecuteSayCommand( const std::string& cmd ) const;

	static std::string GetRankName(UserStatus::RankContainer rank);

    float GetBalanceRank() const;
    UserStatus::RankContainer GetRank() const;
	std::string GetClan();

	//bool operator< ( const ConstUserPtr other ) const { return m_nick < other.GetNick() ; }
	//User& operator= ( const ConstUserPtr other );
protected:
	// User variables

	IServerPtr m_serv;
	BattlePtr m_battle;
	//! copy-semantics?
};

} // namespace LSL {

#endif // LSL_USER_H
