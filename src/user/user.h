#ifndef LSL_USER_H
#define LSL_USER_H

#include "common.h"

namespace LSL {

//! Class containing all the information about a user
class User : public CommonUser
{
public:
	User( Server& serv );
	User( const wxString& nick, Server& serv );
	User( const wxString& nick, const wxString& country, const int& cpu, Server& serv);
	User( const wxString& nick );
	User( const wxString& nick, const wxString& country, const int& cpu );
	User();

	virtual ~User();

	// User interface

	Server& GetServer() const { return *m_serv; }

	void Said( const wxString& message ) const;
	void Say( const wxString& message ) const;
	void DoAction( const wxString& message ) const;

	Battle* GetBattle() const;
	void SetBattle( Battle* battle );

	void SendMyUserStatus() const;
	void SetStatus( const UserStatus& status );
	void SetCountry( const wxString& country );

	bool ExecuteSayCommand( const wxString& cmd ) const;

	static wxString GetRankName(UserStatus::RankContainer rank);

	float GetBalanceRank();
	UserStatus::RankContainer GetRank();
	wxString GetClan();

	int GetFlagIconIndex() const { return m_flagicon_idx; }
	int GetRankIconIndex() const { return m_rankicon_idx; }
	int GetStatusIconIndex() const { return m_statusicon_idx; }
	//bool operator< ( const User& other ) const { return m_nick < other.GetNick() ; }
	//User& operator= ( const User& other );

	int GetSideiconIndex() const { return m_sideicon_idx; }
	void SetSideiconIndex( const int idx ) { m_sideicon_idx = idx; }

protected:
	// User variables

	Server* m_serv;
	Battle* m_battle;
	int m_flagicon_idx;
	int m_rankicon_idx;
	int m_statusicon_idx;
	int m_sideicon_idx;

	//! copy-semantics?
};

} // namespace LSL {

#endif // USER_H
