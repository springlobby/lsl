#ifndef TASSERVER_H
#define TASSERVER_H

#include "iserver.h"

#include <utils/type_forwards.h>
#include <boost/format/format_fwd.hpp>

namespace LSL {

class CommandDictionary;

class TASServer : public iServer {
public:
	TASServer();

	/*** these are only in this class */
	void AcceptAgreement();
	void RequestChannels();
	void Login(const std::string& user, const std::string& password);
	std::string GetPasswordHash(const std::string& pass) const;
	bool IsPasswordHash(const std::string& pass) const;
	int Register(const std::string& addr, const int port, const std::string& nick, const std::string& password);
	void GetLastUserIP(const std::string& user);
	void GetUserIP(const std::string& user);
	void GetLastLoginTime(const std::string& user);
	void Rename(const std::string& newnick);
	void GetMD5(const std::string& text, const std::string& newpassword);
	void ChangePassword(const std::string& oldpassword, const std::string& newpassword);
	void GetChannelMutelist(const std::string& channel);
	void GetIP(const std::string& user);
	void SendChannelMessage(const std::string& channel, const std::string& message);
	void SetChannelTopic(const std::string& channel, const std::string& topic);
	void GetBanList();
	void UnBanUser(const std::string& user);
	void BanUser(const std::string& user);
	void KickUser(const std::string& user);
	void GetInGameTime(const std::string& user);
	/*** **************************** */

	void ModeratorSetChannelTopic(const std::string &channel, const std::string &topic);
	void ModeratorSetChannelKey(const std::string &channel, const std::string &key);
	void ModeratorMute(const std::string &channel, const std::string &nick, int duration, bool byip);
	void ModeratorUnmute(const std::string &channel, const std::string &nick);
	void ModeratorKick(const std::string &channel, const std::string &reason);
	void ModeratorBan(const std::string &, bool);
	void ModeratorUnban(const std::string &);
	void ModeratorGetIP(const std::string &nick);
	void ModeratorGetLastLogin(const std::string &nick);
	void ModeratorGetLastIP(const std::string &nick);
	void ModeratorFindByIP(const std::string &ipadress);
	void AdminGetAccountAccess(const std::string &);

	void SendPing();
	void AdminChangeAccountAccess(const std::string &, const std::string &);
	void AdminSetBotMode(const std::string &nick, bool isbot);
private:
	void OnNewUser( std::string nick, std::string country, int cpu, int id );
	void ExecuteCommand( const std::string& cmd, std::string& inparams );
	void ExecuteCommand( const std::string& cmd, std::string& inparams, int replyid );
	void SendCmd( const std::string& command, const std::string& param = "" );
	void SendCmd( const std::string& command, const boost::format& param );
	virtual void JoinChannel( const std::string& channel, const std::string& key );
	virtual void PartChannel( const std::string& channel );

	virtual void DoActionChannel( const std::string& channel, const std::string& msg );
	virtual void SayChannel( const std::string& channel, const std::string& msg );

	virtual void SayPrivate( const std::string& user, const std::string& msg );
	virtual void DoActionPrivate( const std::string& user, const std::string& msg );

	virtual void SayBattle( const int battle_id, const std::string& msg );
	virtual void DoActionBattle( const int battle_id, const std::string& msg );

	virtual void Ring( const std::string& user );
	virtual void _Disconnect(const std::string& reason);
	virtual void _Ping();
	virtual void _JoinChannel( const std::string& channel, const std::string& key );
	virtual void _JoinBattle( const IBattlePtr battle, const std::string& password, const std::string& scriptpassword );

    friend class CommandDictionary;
    CommandDictionary* m_cmd_dict;
};

} //namespace LSL

#endif // TASSERVER_H
