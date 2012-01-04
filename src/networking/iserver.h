#ifndef LSL_ISERVER_H
#define LSL_ISERVER_H

#include <string>
#include <map>
#include <vector>
#include <boost/signals2/signal.hpp>
#include <utils/type_forwards.h>
#include "enums.h"
#include <utils/mutexwrapper.h>
#include <utils/crc.h>


namespace LSL {

namespace Battle {
class IBattle;
class BattleOptions;
}

class PingThread {};

class ServerEvents;
class Channel;
class User;
class HostInfo;
class UserBattleStatus;
class Socket;
class PingThread;
class IServerEvents;
class UnitsyncMap;
class UnitsyncMod;
class UserStatus;
class MuteList;

class iServer
{
  public:
    iServer();

	boost::signals2::signal<void ()> sig_NATPunchFailed;
	//! battle_id
	boost::signals2::signal<void (int)> sig_StartHostedBattle;
	//! seconds since ping received
	boost::signals2::signal<void (int)> sig_Pong;
	boost::signals2::signal<void ()> sig_Timeout;

	typedef std::vector<std::string>
		StringVector;
	typedef std::vector<UserPtr>
		UserVector;

	// Server interface

	virtual bool ExecuteSayCommand( const std::string& cmd ) = 0;

	virtual bool Register( const std::string& addr, const int port, const std::string& nick, const std::string& password,std::string& reason ) = 0;
	virtual void AcceptAgreement() = 0;

	void Connect( const std::string& servername, const std::string& addr, const int port );
    void Disconnect(const std::string& reason);
    bool IsConnected();

	virtual void Login() = 0;
	virtual void Logout() = 0;
	bool IsOnline()  const ;

	void TimerUpdate();

	virtual void JoinChannel( const std::string& channel, const std::string& key ) = 0;
	virtual void PartChannel( ChannelPtr channel ) = 0;

	virtual void DoActionChannel( const ChannelPtr channel, const std::string& msg ) = 0;
	virtual void SayChannel( const ChannelPtr channel, const std::string& msg ) = 0;

	virtual void SayPrivate( const UserPtr user, const std::string& msg ) ;
	virtual void SayPrivate( const std::string& user, const std::string& msg ) = 0;
	virtual void DoActionPrivate( const UserPtr user, const std::string& msg );
	virtual void DoActionPrivate( const std::string& user, const std::string& msg ) = 0;

	virtual void SayBattle( const BattlePtr battle, const std::string& msg ) = 0;
	virtual void DoActionBattle( const BattlePtr battle, const std::string& msg ) = 0;

	virtual void Ring( const UserPtr user ) = 0;

	// these need to not use specific classes since they can be nonexistent/offline
	virtual void ModeratorSetChannelTopic( const std::string& channel, const std::string& topic ) = 0;
	virtual void ModeratorSetChannelKey( const std::string& channel, const std::string& key ) = 0;
	virtual void ModeratorMute( const std::string& channel, const std::string& nick, int duration, bool byip ) = 0;
	virtual void ModeratorUnmute( const std::string& channel, const std::string& nick ) = 0;
	virtual void ModeratorKick( const std::string& channel, const std::string& reason ) = 0;
	virtual void ModeratorBan( const std::string& nick, bool byip ) = 0;
	virtual void ModeratorUnban( const std::string& nick ) = 0;
	virtual void ModeratorGetIP( const std::string& nick ) = 0;
	virtual void ModeratorGetLastLogin( const std::string& nick ) = 0;
	virtual void ModeratorGetLastIP( const std::string& nick ) = 0;
	virtual void ModeratorFindByIP( const std::string& ipadress ) = 0;

	virtual void AdminGetAccountAccess( const std::string& nick ) = 0;
	virtual void AdminChangeAccountAccess( const std::string& nick, const std::string& accesscode ) = 0;
	virtual void AdminSetBotMode( const std::string& nick, bool isbot ) = 0;

	virtual void HostBattle( Battle::BattleOptions bo ) = 0;
	virtual void JoinBattle( const BattlePtr battle, const std::string& password = "" ) = 0;
	void LeaveBattle( const BattlePtr battle );
	void StartHostedBattle();

	virtual void ForceSide( const BattlePtr battle, const UserPtr user, int side ) = 0;
	virtual void ForceTeam( const BattlePtr battle, const UserPtr user, int team ) = 0;
	virtual void ForceAlly( const BattlePtr battle, const UserPtr user, int ally ) = 0;
	virtual void ForceColor( const BattlePtr battle, const UserPtr user, int r, int g, int b ) = 0;
	virtual void ForceSpectator( const BattlePtr battle, const UserPtr user, bool spectator ) = 0;
	void BattleKickPlayer( const BattlePtr battle, const UserPtr user );
	virtual void SetHandicap( const BattlePtr battle, const UserPtr user, int handicap) = 0;

	virtual void AddBot( const BattlePtr battle, const std::string& nick, UserBattleStatus& status ) = 0;
	virtual void RemoveBot( const BattlePtr battle, const UserPtr user ) = 0;
	virtual void UpdateBot( const BattlePtr battle, const UserPtr user, UserBattleStatus& status ) = 0;

	virtual void SendHostInfo( HostInfo update ) = 0;
	virtual void SendHostInfo( const std::string& Tag ) = 0;
	virtual void SendRaw( const std::string& raw ) = 0;
	virtual void SendUserPosition( const User& usr ) = 0;
	virtual void SendCmd( const std::string& command, const std::string& param ) = 0;

	virtual void RequestInGameTime( const std::string& nick ) = 0;

	virtual BattlePtr GetCurrentBattle() = 0;

	virtual void RequestChannels() = 0;

	virtual void SendMyBattleStatus( UserBattleStatus& bs ) = 0;
	virtual void SendMyUserStatus() = 0;

	void SetKeepaliveInterval( int seconds ) { m_keepalive = seconds; }
	int GetKeepaliveInterval() { return m_keepalive; }

	virtual bool IsPasswordHash( const std::string& pass ) const = 0;
	virtual std::string GetPasswordHash( const std::string& pass ) const = 0;

	std::string GetRequiredSpring() const { return m_min_required_spring_ver; }
	void SetRequiredSpring( const std::string& version ) { m_min_required_spring_ver = version; }

	virtual void OnConnected( Socket* sock ) = 0;
	virtual void OnDisconnected( Socket* sock ) = 0;

	const UserPtr GetMe() const {return m_me;}

	virtual void SendScriptToClients( const std::string& script ) = 0;

	std::string GetServerName() const { return m_server_name; }

	virtual void RequestSpringUpdate();

	void SetRelayIngamePassword( const UserPtr user );

	UserPtr AcquireRelayhost();
	void SendScriptToProxy( const std::string& script );

	void SetPrivateUdpPort(int port) {m_udp_private_port = port;}
	std::string GenerateScriptPassword();
	int RelayScriptSendETA( const std::string& script ); //!in seconds

protected://ideally this would be nothing, so long as Tasserver is still a child
	Socket* m_sock;

private:
	std::map<std::string,std::string> m_channel_pw;  /// channel name -> password, filled on channel join
	//! @brief map used internally by the iServer class to calculate ping roundtimes.
	typedef std::map<int, long long> PingList;

    CRC m_crc;
	int m_keepalive; //! in seconds
	int m_ping_timeout; //! in seconds
	int m_ping_interval; //! in seconds
	int m_server_rate_limit; //! in bytes/sec
	int m_message_size_limit; //! in bytes
	UserPtr m_me;
	std::string m_min_required_spring_ver;
	std::string m_last_denied_connection_reason;
    std::string m_server_name;
	PingThread m_ping_thread;
	std::string m_buffer;
    bool m_connected;
    bool m_online;
    int m_udp_private_port;
    int m_nat_helper_port;
	int m_udp_reply_timeout;
	time_t m_last_udp_ping;

	MutexWrapper<unsigned int> m_last_ping_id;
	unsigned int GetLastPingID()
	{
		ScopedLocker<unsigned int> l_last_ping_id(m_last_ping_id);
        return l_last_ping_id.Get();
	}
	MutexWrapper<PingList> m_pinglist;
	PingList& GetPingList()
	{
		ScopedLocker<PingList> l_pinglist(m_pinglist);
		return l_pinglist.Get();
	}

	BattlePtr m_current_battle;

	UserPtr m_relay_host_bot;
	UserPtr m_relay_host_manager;
	UserPtr m_relay_host_list;

	UserVector GetAvailableRelayHostList();
	StringVector m_relay_host_manager_list;

	UserPtr AddUser( const int id );
	void RemoveUser( const UserPtr user );

	ChannelPtr AddChannel( const std::string& chan );
	void RemoveChannel( const ChannelPtr chan );

	BattlePtr AddBattle( const int& id );
	void RemoveBattle( const BattlePtr battle );

	void RelayCmd( const std::string& command, const std::string& param );
	void OpenBattle( Battle::BattleOptions bo );
	void HandlePong( int replyid );
	void Ping();


private://defs from iserver.cpp bottom
	void OnSocketError( const Enum::SocketError& /*unused*/ );
	void OnProtocolError( const Enum::Protocolerror /*unused*/ );
	void OnNewUser( const UserPtr user );
	void OnUserStatus( const UserPtr user, UserStatus status );
	void OnServerInitialData(const std::string& server_name, const std::string& server_ver, bool supported, const std::string& server_spring_ver, bool /*unused*/);
	void OnBattleStarted( const BattlePtr battle );
	void OnDisconnected( bool wasonline );
	void OnLogin( const UserPtr user );
	void OnLogout();
	void OnLoginInfoComplete();
	void OnUnknownCommand( const std::string& command, const std::string& params );
	void OnMotd( const std::string& msg );
	void OnPong( long long ping_time );
	void OnUserQuit( const UserPtr user );
	void OnBattleOpened( const BattlePtr battle );
	void OnBattleMapChanged(const BattlePtr battle,UnitsyncMap map);
	void OnBattleModChanged( const BattlePtr battle, UnitsyncMod mod );
	void OnBattleMaxPlayersChanged( const BattlePtr battle, int maxplayers );
	void OnBattleHostChanged( const BattlePtr battle, UserPtr host, const std::string& ip, int port );
	void OnBattleSpectatorCountUpdated(const BattlePtr battle,int spectators);
	void OnUserJoinedBattle( const BattlePtr battle, const UserPtr user );
	void OnAcceptAgreement( const std::string& agreement );
	void OnRing( const UserPtr from );
	void OnServerBroadcast( const std::string& message );
	void OnServerMessage( const std::string& message );
	void OnServerMessageBox( const std::string& message );
	void OnChannelMessage( const ChannelPtr channel, const std::string& msg );
	void OnBattleLockUpdated(const BattlePtr battle,bool locked);
	void OnUserLeftBattle(const BattlePtr battle, const UserPtr user);
	void OnBattleClosed(const BattlePtr battle );
	void OnBattleDisableUnit( const BattlePtr battle, const std::string& unitname, int count );
	void OnBattleEnableUnit( int battleid, const StringVector& unitnames );
	void OnBattleEnableAllUnits( int battleid );
	void OnJoinChannelFailed( const ChannelPtr channel, const std::string& reason );
	void OnUserJoinedChannel( const ChannelPtr channel, const UserPtr user );
	void OnKickedFromChannel( const ChannelPtr channel, const std::string& fromWho, const std::string& msg );
	void OnLoginFailed( const std::string& reason );
	void OnChannelSaid( const ChannelPtr channel, const UserPtr user, const std::string& message );
	void OnBattleStartRectAdd( const BattlePtr battle, int allyno, int left, int top, int right, int bottom );
	void OnBattleStartRectRemove( const BattlePtr battle, int allyno );
	void OnFileDownload( bool autolaunch, bool autoclose, bool /*disconnectonrefuse*/, const std::string& FileName, const std::string& url, const std::string& description );
	void OnBattleScript( const BattlePtr battle, const std::string& script );
	void OnMuteList(const ChannelPtr channel, const MuteList& mutelist );
	void OnKickedFromBattle( const BattlePtr battle);
	void OnUserInternalUdpPort( const UserPtr user, int udpport );
	void OnUserExternalUdpPort( const UserPtr user, int udpport );
	void OnUserIP( const UserPtr user, const std::string& ip );
	void OnRedirect( const std::string& address, int port );
	void OnChannelJoinUserList( const ChannelPtr channel, const UserVector& users);
	void OnChannelListEnd();
	void OnJoinBattleFailed( const std::string& msg );
	void OnOpenBattleFailed( const std::string& msg );
	void OnRequestBattleStatus();
};

} //namespace LSL

#endif //LSL_ISERVER_H
