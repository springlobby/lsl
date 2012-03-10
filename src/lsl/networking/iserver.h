#ifndef LSL_ISERVER_H
#define LSL_ISERVER_H

#include <string>
#include <map>
#include <vector>
#include <boost/signals2/signal.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <lslutils/mutexwrapper.h>
#include <lslutils/crc.h>
#include <lsl/battle/enum.h>
#include <lsl/container/battlelist.h>
#include <lsl/container/channellist.h>
#include <lsl/container/userlist.h>

#include <lslutils/type_forwards.h>
#include "enums.h"

namespace LSL {

namespace Battle {
class IBattle;
class BattleOptions;
}

const unsigned int FIRST_UDP_SOURCEPORT = 8300;

class Channel;
class User;
class UserBattleStatus;
class Socket;
class PingThread;
class ServerImpl;
class UnitsyncMap;
class UnitsyncMod;
class UserStatus;
class Server;

struct PingThread { PingThread(Server*,int){}
					PingThread(){}
					void Init(){}};

struct MuteListEntry {
    const ConstUserPtr who;
    const std::string msg;
    MuteListEntry( const ConstUserPtr _who, const std::string _msg )
        : who(_who), msg(_msg)
    {}
};

typedef std::list<MuteListEntry>
    MuteList;

class Server : public boost::enable_shared_from_this<Server>
{
  public:
    Server();
    virtual ~Server();

    friend class ServerImpl;

    boost::signals2::signal<void ()> sig_NATPunchFailed;
    //! battle_id
    boost::signals2::signal<void (int)> sig_StartHostedBattle;
    //! seconds since ping received
    boost::signals2::signal<void (int)> sig_Pong;
    boost::signals2::signal<void ()> sig_Timeout;
    //! success | msg | msg_id
    boost::signals2::signal<void (bool,std::string,int)> sig_SentMessage;
    //! user whose status changed | the changed status
    boost::signals2::signal<void (const ConstUserPtr,UserStatus)> sig_UserStatusChanged;
    //! was_online
    boost::signals2::signal<void (bool)> sig_Disconnected;
    //! the udp port
    boost::signals2::signal<void (int)> sig_MyInternalUdpSourcePort;

	void Connect( const std::string& servername, const std::string& addr, const int port );
    void Disconnect(const std::string& reason);
    bool IsConnected();

    void Logout();
	bool IsOnline()  const ;

	void TimerUpdate();

    void PartChannel( ChannelPtr channel );
    void JoinChannel( const std::string& channel, const std::string& key );
    void SayChannel( const ChannelPtr channel, const std::string& msg );

    void SayPrivate( const ConstUserPtr user, const std::string& msg ) ;
    void DoActionPrivate( const ConstUserPtr user, const std::string& msg );

	// these need to not use specific classes since they can be nonexistent/offline
	void BattleKickPlayer( const IBattlePtr battle, const UserPtr user );

    IBattlePtr GetCurrentBattle();
    const ConstIBattlePtr GetCurrentBattle() const;

    void SetKeepaliveInterval( int seconds );
    int GetKeepaliveInterval();

    std::string GetRequiredSpring() const;
    void SetRequiredSpring( const std::string& version );

    void OnSocketConnected(bool connection_ok, const std::string msg);

    const UserPtr GetMe() const;

    std::string GetServerName() const;

	void SetRelayIngamePassword( const UserPtr user );

	UserPtr AcquireRelayhost();
	void SendScriptToProxy( const std::string& script );

    void SetPrivateUdpPort(int port);
    std::string GenerateScriptPassword() const;
    int RelayScriptSendETA( const std::string& script ); //!in seconds

    void SendMyUserStatus();
    void SendMyBattleStatus(const UserBattleStatus &bs);
    void JoinBattle( const IBattlePtr battle, const std::string& password );
    void SayBattle( const IBattlePtr battle, const std::string& msg );
    void DoActionBattle( const IBattlePtr battle, const std::string& msg );
    void AddBot(const BattlePtr battle, const std::string &nick, UserBattleStatus &status);
    void RemoveBot(const BattlePtr battle, const UserPtr user);
    void UpdateBot(const BattlePtr battle, const UserPtr user, UserBattleStatus &incoming_status);
    void ForceSide(const BattlePtr battle, const UserPtr user, int side);
    void ForceTeam(const BattlePtr battle, const UserPtr user, int team);
    void ForceAlly(const BattlePtr battle, const UserPtr user, int ally);
    void ForceColor(const BattlePtr battle, const UserPtr user, const lslColor& color);
    void ForceSpectator(const BattlePtr battle, const UserPtr user, bool spectator);
    void BattleKickPlayer(const BattlePtr battle, const UserPtr user);
    void SetHandicap(const BattlePtr battle, const UserPtr user, int handicap);
    void SendUserPosition( const UserPtr user );
    void SendScriptToClients(const std::string &script);
    void Ring(const ConstUserPtr user );
    void StartHostedBattle();
    void LeaveBattle( const IBattlePtr battle);
    void SendHostInfo(Enum::HostInfo update);
    void SendHostInfo(const std::string &key);

    void RemoveUser( const UserPtr user );
    void RemoveChannel( const ChannelPtr chan );
    void RemoveBattle( const IBattlePtr battle );

private:
    UserVector GetAvailableRelayHostList();
    void HandlePong( int replyid );

public:

	void OpenBattle( Battle::BattleOptions bo );

	void OnSocketError( const Enum::SocketError& /*unused*/ );
	void OnProtocolError( const Enum::Protocolerror /*unused*/ );
	void OnNewUser( const UserPtr user );
	void OnUserStatus( const UserPtr user, UserStatus status );
    void OnServerInitialData(const std::string& server_name,
                             const std::string& server_ver,
                             bool supported,
                             const std::string& server_spring_ver,
                             bool /*unused*/);
    void OnBattleStarted(const IBattlePtr battle);
    void OnBattleStopped(const IBattlePtr battle);
    void OnDisconnected();
	void OnLogin( const UserPtr user );
	void OnLogout();
	void OnUnknownCommand( const std::string& command, const std::string& params );
	void OnPong( long long ping_time );
	void OnUserQuit( const UserPtr user );
	void OnBattleOpened( const IBattlePtr battle );
	void OnBattleMapChanged(const IBattlePtr battle,UnitsyncMap map);
	void OnBattleModChanged( const IBattlePtr battle, UnitsyncMod mod );
	void OnBattleMaxPlayersChanged( const IBattlePtr battle, int maxplayers );
	void OnBattleHostChanged( const IBattlePtr battle, UserPtr host, const std::string& ip, int port );
	void OnBattleSpectatorCountUpdated(const IBattlePtr battle,int spectators);
	void OnUserJoinedBattle( const IBattlePtr battle, const UserPtr user );
	void OnAcceptAgreement( const std::string& agreement );
	void OnRing( const UserPtr from );
	void OnChannelMessage( const ChannelPtr channel, const std::string& msg );
	void OnBattleLockUpdated(const IBattlePtr battle,bool locked);
	void OnUserLeftBattle(const IBattlePtr battle, const UserPtr user);
	void OnBattleClosed(const IBattlePtr battle );
	void OnBattleDisableUnit( const IBattlePtr battle, const std::string& unitname, int count );
    void OnBattleEnableUnit( const IBattlePtr battle, const StringVector& unitnames );
    void OnBattleEnableAllUnits( const IBattlePtr battle );
	void OnJoinChannelFailed( const ChannelPtr channel, const std::string& reason );
	void OnUserJoinedChannel( const ChannelPtr channel, const UserPtr user );
	void OnKickedFromChannel( const ChannelPtr channel, const std::string& fromWho, const std::string& msg );
	void OnChannelSaid( const ChannelPtr channel, const UserPtr user, const std::string& message );
	void OnBattleStartRectAdd( const IBattlePtr battle, int allyno, int left, int top, int right, int bottom );
	void OnBattleStartRectRemove( const IBattlePtr battle, int allyno );
	void OnFileDownload( bool autolaunch, bool autoclose, bool /*disconnectonrefuse*/, const std::string& FileName, const std::string& url, const std::string& description );
	void OnBattleScript( const IBattlePtr battle, const std::string& script );
	void OnMuteList(const ChannelPtr channel, const MuteList& mutelist );
	void OnKickedFromBattle( const IBattlePtr battle);
	void OnUserInternalUdpPort( const UserPtr user, int udpport );
	void OnUserExternalUdpPort( const UserPtr user, int udpport );
	void OnUserIP( const UserPtr user, const std::string& ip );
    void OnChannelJoinUserList( const ChannelPtr channel, const UserVector& users);
    void OnRequestBattleStatus( IBattlePtr battle );
    void OnSelfHostedBattle( IBattlePtr battle );
    void OnSelfJoinedBattle( IBattlePtr battle );
    void OnSetBattleOption( IBattlePtr battle, const std::string& param, const std::string& value );
    void OnClientBattleStatus( IBattlePtr battle, UserPtr user, UserBattleStatus bstatus );
    void OnBattleEnableUnits( IBattlePtr battle, const StringVector unitlist );
    void OnUserLeftChannel( ChannelPtr channel, UserPtr user );
    void OnUserStartPositionUpdated( IBattlePtr battle, UserPtr player, const UserPosition& pos );
    void OnChannelPart( ChannelPtr channel, UserPtr user, const std::string& message );
    void OnChannelTopic( ChannelPtr channel, UserPtr user, const std::string& message );
    void OnChannelAction( ChannelPtr channel, UserPtr user, const std::string& action );

	void OnUserScriptPassword( const UserPtr user, const std::string& pw );
    void OnBattleHostchanged( IBattlePtr battle, int udpport );
    void OnUserBattleStatusUpdated( IBattlePtr battle, UserPtr user, const UserBattleStatus& status );

    int GetNextAvailableID();

private:
	void UdpPingTheServer( const std::string& message = "" );/// used for nat travelsal. pings the server.
	//! used when hosting with nat holepunching. has some rudimentary support for fixed source ports.
	void UdpPingAllClients();
	//! full parameters version, used to ping all clients when hosting.
	unsigned int UdpPing(unsigned int src_port, const std::string &target, unsigned int target_port, const std::string &message);
    //! toogle == true -> RelayCmd, else SendCmd
    void SendOrRelayCmd( bool toggle, const std::string& command, const std::string& param );

    ServerImpl* m_impl;
};

} //namespace LSL

/**
 * \file iserver.h
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

#endif //LSL_ISERVER_H
