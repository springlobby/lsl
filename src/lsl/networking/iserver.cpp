#include "iserver.h"

#include "socket.h"
#include "commands.h"
#include "tasserverdataformats.h"

#include <lsl/battle/ibattle.h>
#include <lsl/user/user.h>

#include <boost/typeof/typeof.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>


namespace LSL {

iServer::iServer()
    : m_keepalive(15),
    m_ping_timeout(40),
    m_ping_interval(10),
    m_server_rate_limit(800),
    m_message_size_limit(1024),
    m_connected(false),
    m_online(false),
	//m_last_udp_ping(0),
    m_udp_private_port(0),
    m_udp_reply_timeout(0),
    m_buffer(""),
    m_sock( new Socket() )
{
    m_sock->sig_doneConnecting.connect(
                boost::bind( &iServer::OnSocketConnected, this, _1, _2 )
                );
}

iServer::~iServer()
{
    delete m_sock;
}

void iServer::Connect( const std::string& servername ,const std::string& addr, const int port )
{
	m_buffer = "";
	m_sock->Connect( addr, port );
    m_sock->SetSendRateLimit( m_server_rate_limit );
	m_connected = false;
    m_online = false;
//    m_redirecting = false;
//    m_agreement = "";
	m_crc.ResetCRC();
	std::string handle = m_sock->GetHandle();
    if ( handle.length() > 0 ) m_crc.UpdateData( handle + addr );
}

void iServer::Disconnect(const std::string& reason)
{
    if (!m_connected)
    {
        return;
    }
	_Disconnect(reason);
	m_sock->Disconnect();
}

bool iServer::IsOnline() const
{
	if ( !m_connected ) return false;
	return m_online;
}

bool iServer::IsConnected()
{
	return (m_sock->State() == Enum::SS_Open);
}

void iServer::TimerUpdate()
{
	if ( !IsConnected() )
		return;
	if ( m_sock->InTimeout( m_ping_timeout ) )
	{
		sig_Timeout();
		Disconnect("timeout");
		return;
	}

	// joining battle with nat traversal:
	// if we havent finalized joining yet, and udp_reply_timeout seconds has passed since
	// we did UdpPing(our name) , join battle anyway, but with warning message that nat failed.
	// (if we'd receive reply from server, we'd finalize already)
	//
	time_t now = time(0);
	if (m_last_udp_ping > 0 && (now - m_last_udp_ping) > m_udp_reply_timeout )
	{
		sig_NATPunchFailed();
	}

	// Is it time for a nat traversal PING?
	if ( ( m_last_udp_ping + m_keepalive ) < now )
	{
		m_last_udp_ping = now;
		// Nat travelsal "ping"
		const ConstIBattlePtr battle=GetCurrentBattle();
		if (battle && !battle->InGame() )
		{
			if ( battle->GetNatType() == Enum::NAT_Hole_punching
				 || battle->GetNatType() == Enum::NAT_Fixed_source_ports  )
			{
				UdpPingTheServer();
				if ( battle->IsFounderMe() )
				{
					UdpPingAllClients();
				}
			}
		}
	}
}

void iServer::SayChannel(const ChannelPtr channel, const std::string &msg)
{
	SayChannel( channel->Name(), msg );
}

void iServer::Ping()
{
	_Ping();
	GetPingList()[GetLastPingID()] = time(0);
}

void iServer::HandlePong( int replyid )
{
	PingList& pinglist = GetPingList();
	PingList::iterator itor = pinglist.find(replyid);
	if ( itor != pinglist.end() )
	{
		sig_Pong( time(0) - itor->second );
		pinglist.erase( itor );
    }
}

void iServer::JoinChannel( const std::string& channel, const std::string& key )
{
    m_channel_pw[channel] = key;
	_JoinChannel(channel,key);
}

UserPtr iServer::AcquireRelayhost()
{
    const unsigned int numbots = m_relay_masters.size();
	if ( numbots > 0 )
	{
		srand ( time(NULL) );
		const unsigned int choice = rand() % numbots;
        m_relay_host_manager = m_relay_masters[choice];
		SayPrivate( m_relay_host_manager, "!spawn" );
		return m_relay_host_manager;
	}
	return UserPtr();
}

void iServer::OpenBattle( Battle::BattleOptions bo )
{
	if ( bo.userelayhost )
	{
		AcquireRelayhost();
		m_last_relay_host_password = bo.password;
	}

	if ( bo.nattype > 0 )
		UdpPingTheServer();
	_HostBattle(bo);
}

std::string GenerateScriptPassword()
{
	char buff[8];
	sprintf(buff,"%04x%04x", rand()&0xFFFF, rand()&0xFFFF);
	return std::string(buff);
}

void iServer::JoinBattle( const IBattlePtr battle, const std::string& password )
{
	if (battle)
	{
		if ( battle->GetNatType() == Enum::NAT_Hole_punching
			 || battle->GetNatType() == Enum::NAT_Fixed_source_ports )
		{
			for (int n=0;n<5;++n) // do 5 udp pings with tiny interval
			{
				UdpPingTheServer( GetMe()->Nick() );
				m_last_udp_ping = time(0);
			}
		}
		srand ( time(NULL) );
		_JoinBattle(battle,password,GenerateScriptPassword());
	}
}


void iServer::StartHostedBattle()
{
	if (!m_current_battle) return;
	if (!m_current_battle->IsFounderMe()) return;
	if ( m_current_battle->GetNatType() == Enum::NAT_Hole_punching
		 || m_current_battle->GetNatType() == Enum::NAT_Fixed_source_ports )
	{
		UdpPingTheServer();
		for (int i=0;i<5;++i)
		{
			UdpPingAllClients();
		}
	}
	_StartHostedBattle();
    sig_StartHostedBattle( m_current_battle->Id() );
}

void iServer::LeaveBattle( const IBattlePtr battle)
{
	m_relay_host_bot = UserPtr();
    _LeaveBattle(battle);
}

void iServer::BattleKickPlayer( const IBattlePtr battle, const UserPtr user )
{
	if (!battle) return;
	if (!battle->IsFounderMe()) return;
	if (!battle->IsProxy()) return;
	if (!user) return;
	user->BattleStatus().scriptPassword = GenerateScriptPassword(); // reset user script password, so he can't rejoin
	SetRelayIngamePassword( user );
}


UserVector iServer::GetAvailableRelayHostList()
{
    if ( m_relay_host_manager )
	{
		// this isn't blocking... so what is good for here?
        SayPrivate( m_relay_host_manager, "!listmanagers" );
	}
    UserVector ret;
    for ( unsigned int i = 0; i < m_relay_masters.size(); i++ )
	{
        UserPtr manager = m_relay_masters[i];
		// skip the manager is not connected or reports it's ingame ( no slots available ), or it's away ( functionality disabled )
		if (!manager) continue;
		if ( manager->Status().in_game ) continue;
		if ( manager->Status().away ) continue;
		ret.push_back( manager );
	}
	return ret;
}

void iServer::RelayCmd(  const std::string& command, const std::string& param )
{
	if ( m_relay_host_bot )
	{
		SayPrivate( m_relay_host_bot, "!" + command + " " + param );
	}
	else
	{
		SendCmd( command, param );
	}
}

void iServer::RelayCmd( const std::string& command, const boost::format& param )
{
    RelayCmd( command, param.str() );
}

void iServer::SetRelayIngamePassword( const UserPtr user )
{
	if (!user) return;
	if (!m_current_battle) return;
	if ( !m_current_battle->InGame() ) return;
		RelayCmd( "SETINGAMEPASSWORD", user->Nick() + " " + user->BattleStatus().scriptPassword );
}

int iServer::RelayScriptSendETA(const std::string& script)
{
    const StringVector strings = Util::StringTokenize( script, "\n");
	int relaylengthprefix = 10 + 1 + m_relay_host_bot->Nick().length() + 2; // SAYPRIVATE + space + botname + space + exclamation mark length
	int length = script.length();
	length += relaylengthprefix + 11 + 1; // CLEANSCRIPT command size
	length += strings.size() * ( relaylengthprefix + 16 + 1 ); // num lines * APPENDSCRIPTLINE + space command size ( \n is already counted in script.size)
	length += relaylengthprefix + 9 + 1; // STARTGAME command size
	return length / m_sock->GetSendRateLimit(); // calculate time in seconds to upload script
}

void iServer::SendScriptToProxy( const std::string& script )
{
    const StringVector strings = Util::StringTokenize( script, "\n" );
	RelayCmd( "CLEANSCRIPT" );
	for (StringVector::const_iterator itor; itor != strings.end(); itor++)
	{
		RelayCmd( "APPENDSCRIPTLINE", *itor );
	}
	RelayCmd( "STARTGAME" );
}

//! @brief Send udp ping.
//! @note used for nat travelsal.

unsigned int iServer::UdpPing(unsigned int src_port, const std::string &target, unsigned int target_port, const std::string &message)
{
	unsigned int result = 0;
	assert( false );
	return result;
}

void iServer::UdpPingTheServer(const std::string &message)
{
	unsigned int port = UdpPing( m_udp_private_port, m_addr, m_nat_helper_port,message);
	if ( port>0 )
	{
		m_udp_private_port = port;
        sig_MyInternalUdpSourcePort( m_udp_private_port );
	}
}

// copypasta from spring.cpp , to get users ordered same way as in tasclient.
struct UserOrder
{
	int index;// user number for m_users.Get
	int order;// user order (we'll sort by it)
	bool operator<(UserOrder b) const  // comparison function for sorting
	{
		return order<b.order;
	}
};


void iServer::UdpPingAllClients()
{
	if (!m_current_battle)return;
	if (!m_current_battle->IsFounderMe())return;

	// I'm gonna mimic tasclient's behavior.
	// It of course doesnt matter in which order pings are sent,
	// but when doing "fixed source ports", the port must be
	// FIRST_UDP_SOURCEPORT + index of user excluding myself
	// so users must be reindexed in same way as in tasclient
	// to get same source ports for pings.


	// copypasta from spring.cpp
    UserVector ordered_users = m_current_battle->Players();
	//TODO this uses ptr diff atm
	std::sort(ordered_users.begin(),ordered_users.end());

    int i = -1;
    BOOST_FOREACH( const ConstUserPtr user, ordered_users )
	{
        i++;
		if (!user)
			continue;
		const UserBattleStatus& status = user->BattleStatus();
        const std::string ip = status.ip;
        unsigned int port = status.udpport;
		const unsigned int src_port = m_udp_private_port;
		if ( m_current_battle->GetNatType() == Enum::NAT_Fixed_source_ports )
		{
			port = FIRST_UDP_SOURCEPORT + i;
		}

		if (port != 0 && ip.length() )
		{
			UdpPing(src_port, ip, port, "hai!" );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/////                       Internal Server Events                         /////
////////////////////////////////////////////////////////////////////////////////

void iServer::OnSocketConnected(bool connection_ok, const std::string msg)
{
    assert( connection_ok );//add proper error handling
    m_connected = connection_ok;
	m_online = false;
	m_last_udp_ping = 0;
    m_min_required_spring_ver = "";
    m_relay_masters.clear();
	GetLastPingID() = 0;
	GetPingList().clear();
}

void iServer::OnDisconnected(Socket* /*sock*/)
{
	bool connectionwaspresent = m_online || !m_last_denied.length() || m_redirecting;
	m_connected = false;
	m_online = false;
    m_redirecting = false;
    m_last_denied = "";
    m_min_required_spring_ver = "";
    m_relay_masters.clear();
	GetLastPingID() = 0;
	GetPingList().clear();
	// delete all users, battles, channels
	sig_Disconnected( connectionwaspresent );
}

void iServer::OnSocketError( const Enum::SocketError& /*unused*/ )
{
}


void iServer::OnProtocolError( const Enum::Protocolerror /*unused*/ )
{
}

////////////////////////////////////////////////////////////////////////////////
/////                        Command Server Events                         /////
////////////////////////////////////////////////////////////////////////////////
void iServer::OnServerInitialData(const std::string& server_name, const std::string& server_ver, bool supported, const std::string& server_spring_ver, bool /*unused*/)
{
	m_server_name = server_name;
	m_server_ver = server_ver;
    m_min_required_spring_ver = server_spring_ver;
}

void iServer::OnNewUser( const UserPtr user )
{
    if (user->Nick() == "RelayHostManagerList" )
	{
        m_relay_host_manager = user;
        SayPrivate( m_relay_host_manager, "!lm" );
	}
}

void iServer::OnUserStatus( const UserPtr user, UserStatus status )
{
	if ( !user ) return;
    UserStatus oldStatus = user->Status();
	user->SetStatus( status );
	//TODO: event
}

void iServer::OnBattleStarted(const IBattlePtr battle)
{
    if (!battle) return;
    //TODO: event
}

void iServer::OnBattleStopped(const IBattlePtr battle)
{
    if (!battle) return;
    //TODO: event
}

void iServer::OnDisconnected( bool wasonline )
{
	//TODO: event
}

void iServer::OnLogin(const UserPtr user )
{
	if (m_online)
		return;
	m_online = true;
	m_me = user;
	m_ping_thread = new PingThread( *this, m_ping_interval*1000 );
	m_ping_thread->Init();
	//TODO: event
}

void iServer::OnLogout()
{
	//TODO: event
}

void iServer::OnLoginInfoComplete()
{
	//TODO: event
}

void iServer::OnUnknownCommand( const std::string& command, const std::string& params )
{
	//TODO: log
	//TODO: event
}

void iServer::OnMotd( const std::string& msg )
{
	//TODO: event
}

void iServer::OnPong( long long ping_time )
{
	//TODO: event
}

void iServer::OnUserQuit(const UserPtr user)
{
	if ( !user ) return;
	if (user == m_me) return;
	RemoveUser( user );
	//TODO: event
}

void iServer::OnBattleOpened(const IBattlePtr battle )
{
	if ( battle && battle->GetFounder() == m_relay_host_bot )
	{
		battle->SetProxy( m_relay_host_bot->Nick() );
		JoinBattle( battle, m_last_relay_host_password ); // autojoin relayed host battles
	}
}

void iServer::OnBattleMapChanged(const IBattlePtr battle, UnitsyncMap map)
{
	if (!battle) return;
	battle->SetHostMap( map.name, map.hash );
}

void iServer::OnBattleModChanged( const IBattlePtr battle, UnitsyncMod mod )
{
	if (!battle) return;
	battle->SetHostMod( mod.name, mod.hash );
}

void iServer::OnBattleMaxPlayersChanged( const IBattlePtr battle, int maxplayers )
{
	if (!battle) return;
	battle->SetMaxPlayers( maxplayers );
}

void iServer::OnBattleHostChanged( const IBattlePtr battle, UserPtr host, const std::string& ip, int port )
{
	if (!battle) return;
    if (!host) battle->SetFounder( host->Nick() );
	battle->SetHostIp( ip );
	battle->SetHostPort( port );
}

void iServer::OnBattleSpectatorCountUpdated(const IBattlePtr battle,int spectators)
{
	if (!battle) return;
    battle->SetSpectators(spectators);
}

void iServer::OnUserJoinedBattle( const IBattlePtr battle, const UserPtr user )
{
	if (!battle) return;
	if (!user) return;
	if (battle->IsProxy()) RelayCmd("SUPPORTSCRIPTPASSWORD"); // send flag to relayhost marking we support script passwords
}

void iServer::OnAcceptAgreement( const std::string& agreement )
{

}

void iServer::OnRing( const UserPtr from )
{

}

void iServer::OnServerBroadcast( const std::string& message )
{

}

void iServer::OnServerMessage( const std::string& message )
{

}

void iServer::OnServerMessageBox( const std::string& message )
{
}

void iServer::OnChannelMessage( const ChannelPtr channel, const std::string& msg )
{
	if (!channel) return;
}

void iServer::OnBattleLockUpdated(const IBattlePtr battle,bool locked)
{
	if (!battle) return;
	battle->SetLocked(locked);
}

void iServer::OnUserLeftBattle(const IBattlePtr battle, const UserPtr user)
{
	if (!user) return;
	bool isbot = user->BattleStatus().IsBot();
	user->BattleStatus().scriptPassword.clear();
	if (!battle) return;
	battle->OnUserRemoved( user );
	if (user == m_me)
	{
		m_relay_host_bot = UserPtr();
	}
	//TODO: event
}

void iServer::OnBattleClosed(const IBattlePtr battle )
{
    RemoveBattle( battle );
	//TODO:event
}

void iServer::OnBattleDisableUnit( const IBattlePtr battle, const std::string& unitname, int count )
{
	if (!battle) return;
	battle->RestrictUnit( unitname, count );
	//TODO: event
}

void iServer::OnBattleEnableUnit( const IBattlePtr battle, const StringVector& unitnames )
{
	if (!battle) return;
	BOOST_FOREACH( const std::string unit, unitnames )
	{
		battle->UnrestrictUnit( unit );
	}
	//TODO: event
}

void iServer::OnBattleEnableAllUnits(const IBattlePtr battle )
{
	if (!battle) return;
	battle->UnrestrictAllUnits();
	//TODO: event
}


void iServer::OnJoinChannelFailed( const ChannelPtr channel, const std::string& reason )
{
	if (!channel) return;
	//TODO: event
}

void iServer::OnUserJoinedChannel( const ChannelPtr channel, const UserPtr user )
{
	if (!channel) return;
	if (!user) return;
	channel->OnChannelJoin( user );
	//TODO: event
}

void iServer::OnKickedFromChannel( const ChannelPtr channel, const std::string& fromWho, const std::string& msg )
{
	if (!channel) return;
}

void iServer::OnLoginFailed( const std::string& reason )
{
	//TODO: check if disconnect can be removed
	Disconnect("Login Failed: "+reason);
}

void iServer::OnChannelSaid( const ChannelPtr channel, const UserPtr user, const std::string& message )
{
	if ( m_relay_host_bot != 0
         && channel == m_channels.Get( "U" + Util::ToString(m_relay_host_bot->Id()) ) )
	{
		if ( user == m_me && message.length() > 0 && message[0] == '!')
			return;
		if ( user == m_relay_host_bot )
		{
			if ( boost::starts_with(message, "UserScriptPassword") )
			{
				std::string msg_copy;
				GetWordParam( msg_copy ); // skip the command keyword
				std::string usernick = GetWordParam( msg_copy );
				std::string userScriptPassword = GetWordParam( msg_copy );
				UserPtr usr = m_users.Get(usernick);
				if (!usr) return;
				OnUserScriptPassword(user, userScriptPassword);
				return;
			}
		}
	}
	if ( m_relay_host_manager != 0
         && channel == m_channels.Get( "U" + Util::ToString(m_relay_host_manager->Id()) ) )
	{
		if ( user == m_me &&  message.length() > 0 && message[0] == '!' )
			return;
		if ( user == m_relay_host_manager )
		{
			if ( message.length() > 0 && message[0] == '\001' ) // error code
			{
			}
			else
			{
                m_relay_host_bot = m_users.FindByNick( message );
				return;
			}
		}
	}
//    makes no sense to me
//    if ( m_relay_masters.size() > 0
//         && channel == m_channels.Get( "U" + Util::ToString(m_relay_masters.Id() ) ) )
//	{
//		if ( user == m_me && message == "!lm" )
//			return;
//        if ( user == m_relay_masters )
//		{
//			if ( boost::starts_with(message,std::string("list ") ) )
//			{
//                std::string list = Util::AfterFirst( message, " " ) ;
//                m_relay_masters = Util::StringTokenize( list, "\t" );
//                return;
//			}
//		}
//	}
}

void iServer::OnBattleStartRectAdd( const IBattlePtr battle, int allyno, int left, int top, int right, int bottom )
{
	if(!battle) return;
	battle->AddStartRect( allyno, left, top, right, bottom );
	battle->StartRectAdded( allyno );
}

void iServer::OnBattleStartRectRemove( const IBattlePtr battle, int allyno )
{
	if (!battle) return;
	battle->RemoveStartRect( allyno );
	battle->StartRectRemoved( allyno );
}

void iServer::OnFileDownload( bool autolaunch, bool autoclose, bool /*disconnectonrefuse*/, const std::string& FileName, const std::string& url, const std::string& description )
{
    // HUH?
//	UTASOfferFileData parsingdata;
//	parsingdata.data = GetIntParam( params );
}

void iServer::OnBattleScript( const IBattlePtr battle, const std::string& script )
{
	if (!battle) return;
	battle->GetBattleFromScript( true );
}

void iServer::OnMuteList(const ChannelPtr channel, const MuteList& mutelist )
{
}

void iServer::OnKickedFromBattle( const IBattlePtr battle)
{
	if (!battle) return;
}


void iServer::OnUserInternalUdpPort( const UserPtr user, int udpport )
{
	if (!user) return;
}

void iServer::OnUserExternalUdpPort( const UserPtr user, int udpport )
{
	if (!user) return;
    user->BattleStatus().udpport = udpport;
}

void iServer::OnUserIP( const UserPtr user, const std::string& ip )
{
	if (!user) return;
	user->BattleStatus().ip = ip;
}

void iServer::OnRedirect( const std::string& address, int port )
{
	if (!address.length()) return;
	if (!port) return;
}

void iServer::OnChannelJoinUserList( const ChannelPtr channel, const UserVector& users)
{

}

void iServer::OnChannelListEnd()
{
}

void iServer::OnJoinBattleFailed( const std::string& msg )
{
}


void iServer::OnOpenBattleFailed( const std::string& msg )
{
}

void iServer::OnSelfHostedBattle(IBattlePtr battle )
{
}

void iServer::OnSelfJoinedBattle( IBattlePtr battle )
{
}

void iServer::OnRequestBattleStatus(IBattlePtr battle)
{
//	if(!m_battle) return;
}

void iServer::OnUserScriptPassword(const UserPtr user, const std::string &pw)
{
    assert( false );
}

void iServer::OnBattleHostchanged(IBattlePtr battle, int udpport)
{
}

void iServer::OnUserBattleStatusUpdated(IBattlePtr battle, UserPtr user, const UserBattleStatus &status)
{
}

int iServer::GetNextAvailableID()
{
    return 1;
}

void iServer::SayPrivate( const UserPtr user, const std::string& msg )
{
	SayPrivate( user->Nick(), msg );
}

void iServer::DoActionPrivate( const UserPtr user, const std::string& msg )
{
	DoActionPrivate( user->Nick(), msg );
}

} // namespace LSL
