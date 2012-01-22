#ifndef LIBLIBSPRINGLOBBY_HEADERGUARD_COMMON_USER_H
#define LIBLIBSPRINGLOBBY_HEADERGUARD_COMMON_USER_H

/** \file common.h
    \copyright GPL v2 **/

#include <lslutils/global_interfaces.h>
#include <lslutils/type_forwards.h>
#include <lslutils/misc.h>
#include <lsl/user/userdata.h>
#include <string>

namespace LSL {

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

    const std::string& Id() const { return m_id; }
	void SetID( const std::string& id ) { m_id = id; }

	UserStatus& Status() { return m_status; }
    const UserStatus& Status() const { return m_status; }
	virtual void SetStatus( const UserStatus& status );

	UserBattleStatus& BattleStatus() { return m_bstatus; }
    const UserBattleStatus& BattleStatus() const { return m_bstatus; }

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

#endif // LIBLIBSPRINGLOBBY_HEADERGUARD_COMMON_USER_H
