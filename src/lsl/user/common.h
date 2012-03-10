#ifndef LIBLIBSPRINGLOBBY_HEADERGUARD_COMMON_USER_H
#define LIBLIBSPRINGLOBBY_HEADERGUARD_COMMON_USER_H

#include <lslutils/global_interfaces.h>
#include <lslutils/type_forwards.h>
#include <lslutils/misc.h>
#include <lsl/user/userdata.h>

#include <boost/enable_shared_from_this.hpp>
#include <string>

namespace LSL {

static const int DEFAULT_CPU_ID = 9001;


//! parent class leaving out server related functionality
class CommonUser : public HasKey< std::string >,public boost::enable_shared_from_this<CommonUser>
{
public:
    CommonUser(const std::string id = GetNewUserId(),
               const std::string nick = "invalid",
               const std::string country = "",
               const int cpu = DEFAULT_CPU_ID );
	virtual ~CommonUser(){}

    //! provide ids from a pool in case server doesn't send one/we create a local bot that needs one
    static std::string GetNewUserId();

    std::string key() const {return Id();}
	static std::string className() { return "Channel"; }

	const std::string& Nick() const { return m_nick; }
	virtual void SetNick( const std::string& nick ) { m_nick = nick; }

	const std::string& GetCountry() const { return m_country; }
	virtual void SetCountry( const std::string& country ) { m_country = country; }

	int GetCpu() const { return m_cpu; }
	void SetCpu( const int& cpu ) { m_cpu = cpu; }

    const std::string& Id() const { return m_id; }

	UserStatus& Status() { return m_status; }
    const UserStatus& Status() const { return m_status; }
	virtual void SetStatus( const UserStatus& status );
    virtual void SendMyUserStatus() const {}

	UserBattleStatus& BattleStatus() { return m_bstatus; }
    const UserBattleStatus& BattleStatus() const { return m_bstatus; }

	//void SetBattleStatus( const UserBattleStatus& status );/// dont use this to avoid overwriting data like ip and port, use following method.
	void UpdateBattleStatus( const UserBattleStatus& status );

	bool Equals( const CommonUser& other ) const { return ( m_nick == other.Nick() ); }

    const IBattlePtr GetBattle() const;
    void SetBattle( IBattlePtr battle );
    float GetBalanceRank() const;
    virtual std::string GetClan() const { return ""; }
    virtual UserStatus::RankContainer GetRank() const { return UserStatus::RANK_1; }

protected:
	std::string m_nick;
	std::string m_country;
    const std::string m_id;
	int m_cpu;
	UserStatus m_status;
	UserBattleStatus m_bstatus;
    IBattlePtr m_battle;
};

} // namespace LSL

/**
 * \file common.h
 * \section LICENSE
Copyright 2012 by The libSpringLobby team. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#endif // LIBLIBSPRINGLOBBY_HEADERGUARD_COMMON_USER_H
