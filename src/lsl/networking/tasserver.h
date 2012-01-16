#ifndef TASSERVER_H
#define TASSERVER_H

#include "iserver.h"

#include <lslutils/type_forwards.h>
#include <boost/format/format_fwd.hpp>

namespace LSL {

class CommandDictionary;

class TASServer : public iServer {
public:
	TASServer();

	void AcceptAgreement();
	void RequestChannels();
	/*** these are only in this class */
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
	void AdminChangeAccountAccess(const std::string &, const std::string &);
	void AdminSetBotMode(const std::string &nick, bool isbot);

	void SendPing();

	virtual void LeaveBattle( const int& battle_id );
	virtual void StartHostedBattle();
	void SendHostInfo(Enum::HostInfo update);
	void SendHostInfo(int type, const std::string &key);
	void SendMyBattleStatus(UserBattleStatus &bs);
	void SendMyUserStatus();
	void ForceSide(const BattlePtr battle, const UserPtr user, int side);
	void ForceTeam(const BattlePtr battle, const UserPtr user, int team);
	void ForceAlly(const BattlePtr battle, const UserPtr user, int ally);
	void ForceColor(const BattlePtr battle, const UserPtr user, int r, int g, int b);
	void ForceSpectator(const BattlePtr battle, const UserPtr user, bool spectator);
	void BattleKickPlayer(const BattlePtr battle, const UserPtr user);
	void SetHandicap(const BattlePtr battle, const UserPtr user, int handicap);
	void AddBot(const BattlePtr battle, const std::string &nick, UserBattleStatus &status);
	void RemoveBot(const BattlePtr battle, const UserPtr user);
	void UpdateBot(const BattlePtr battle, const UserPtr user, UserBattleStatus &incoming_status);
	void SendScriptToClients(const std::string &script);
	void RequestSpringUpdate(std::string &currentspringversion);
	int GetNewUserId();

	std::string GetBattleChannelName(const BattlePtr battle);

private:
	void ExecuteCommand( const std::string& cmd, std::string& inparams );
	void ExecuteCommand( const std::string& cmd, std::string& inparams, int replyid );

	void OnNewUser( const std::string& nick, const std::string& country, int cpu, int id );


	void SendCmd( const std::string& command, const std::string& param = "" );
	void SendCmd( const std::string& command, const boost::format& param );
	void SendUserPosition(const UserPtr user);
	void SendRaw(const std::string &raw);
	void RequestInGameTime(const std::string &nick);

	virtual void JoinChannel( const std::string& channel, const std::string& key );
	virtual void PartChannel( const std::string& channel );

	virtual void DoActionChannel( const std::string& channel, const std::string& msg );
	virtual void SayChannel( const std::string& channel, const std::string& msg );

	virtual void SayPrivate( const std::string& user, const std::string& msg );
	virtual void DoActionPrivate( const std::string& user, const std::string& msg );

	virtual void SayBattle( const int battle_id, const std::string& msg );
	virtual void DoActionBattle( const int battle_id, const std::string& msg );

    virtual void Ring(const ConstUserPtr user );
	virtual void _Disconnect(const std::string& reason);
	virtual void _Ping();
	virtual void _JoinChannel( const std::string& channel, const std::string& key );
	virtual void _JoinBattle( const IBattlePtr battle, const std::string& password, const std::string& scriptpassword );
	virtual void _HostBattle( Battle::BattleOptions bo );
	virtual void _StartHostedBattle();
	virtual void _LeaveBattle( const IBattlePtr battle);

    friend class CommandDictionary;
    CommandDictionary* m_cmd_dict;

	MutexWrapper<unsigned int> m_last_id;
	unsigned int& GetLastID()
	{
		ScopedLocker<unsigned int> l_last_id(m_last_id);
		return l_last_id.Get();
	}

    std::string m_delayed_open_command;
    std::string m_agreement;
    unsigned int m_account_id_count;
    MuteList m_mutelist;
    std::string m_mutelist_current_channelname;

private:
	//! command handlers
	void OnBattleOpened(int id, Enum::BattleType type, Enum::NatType nat, const std::string &nick, const std::string &host, int port, int maxplayers, bool haspass, int rank, const std::string &maphash, const std::string &map, const std::string &title, const std::string &mod);
	void OnUserStatusChanged(const std::string &nick, int intstatus);
	void OnHostedBattle(int battleid);
	void OnUserQuit(const std::string &nick);
	void OnSelfJoinedBattle(int battleid, const std::string &hash);
	void OnStartHostedBattle();
	void OnClientBattleStatus(const std::string &nick, int intstatus, int colorint);
	void OnUserJoinedBattle(int battleid, const std::string &nick, const std::string &userScriptPassword);
	void OnUserLeftBattle(int battleid, const std::string &nick);
    void OnBattleInfoUpdated(int battleid, int spectators, bool locked, const std::string &maphash, const std::string &mapname);
    void OnSetBattleOption(std::string key, const std::string &value);
    void OnSetBattleInfo(std::string infos);
	void OnAcceptAgreement();
	void OnBattleClosed(int battleid);
	void OnBattleDisableUnits(const std::string &unitlist);
	void OnBattleDisableUnit(const std::string &unitname, int count);
	void OnBattleEnableUnits(const std::string &unitnames);
	void OnBattleEnableAllUnits();
	void OnJoinChannel(const std::string &channel, const std::string& rest);
	void OnJoinChannelFailed(const std::string &channel, const std::string &reason);
	void OnChannelJoin(const std::string &name, const std::string &who);
	void OnChannelJoinUserList(const std::string &channel, const std::string &usernames);
	void OnJoinedBattle(const int battleid, const std::string msg);
	void OnGetHandle();
	void OnLogin(const std::string& msg);
	void OnUserJoinedChannel(const std::string &channel_name, const std::string &who);
	void OnChannelSaid(const std::string &channel, const std::string &who, const std::string &message);
	void OnChannelPart(const std::string &channel, const std::string &who, const std::string &message);
	void OnChannelTopic(const std::string &channel, const std::string &who, int, const std::string &message);
	void OnChannelAction(const std::string &channel, const std::string &who, const std::string &action);
	ChannelPtr GetCreatePrivateChannel(const UserPtr user);
	void OnSayPrivateMessageEx(const std::string &user, const std::string &action);
	void OnSaidPrivateMessageEx(const std::string &user, const std::string &action);
	void OnSayPrivateMessage(const std::string &user, const std::string &message);
	void OnSaidPrivateMessage(const std::string &user, const std::string &message);
	void OnSaidBattle(const std::string &nick, const std::string &msg);
	void OnBattleAction(const std::string &nick, const std::string &msg);
	void OnBattleStartRectAdd(int allyno, int left, int top, int right, int bottom);
	void OnBattleStartRectRemove(int allyno);
	void OnScriptStart();
	void OnScriptLine(const std::string &line);
	void OnScriptEnd();
	void OnMutelistBegin(const std::string &channel);
    void OnMutelistItem(const std::string &mutee, const std::string &message);
	void OnMutelistEnd();
	void OnChannelMessage(const std::string &channel, const std::string &msg);
	void OnRing(const std::string &from);
	void OnKickedFromBattle();
    void OnKickedFromChannel(const std::string &channel, const std::string &fromWho, const std::string &message);
	void OnMyInternalUdpSourcePort(const unsigned int udpport);
	void OnMyExternalUdpSourcePort(const unsigned int udpport);
	void OnClientIPPort(const std::string &username, const std::string &ip, unsigned int udpport);
	void OnHostExternalUdpPort(const int udpport);
	void OnChannelListEntry(const std::string &channel, const int &numusers, const std::string &topic);
	void OnAgreenmentLine(const std::string &line);
	void OnRequestBattleStatus();
	void OnBattleAddBot(int battleid, const std::string &nick, const std::string &owner, int intstatus, int intcolor, const std::string &aidll);
	void OnBattleUpdateBot(int battleid, const std::string &nick, int intstatus, int intcolor);
	void OnBattleRemoveBot(int battleid, const std::string &nick);
	void OnFileDownload(int intdata, const std::string &FileName, const std::string &url, const std::string &description);
};

} //namespace LSL

#endif // TASSERVER_H
