#include "iserver.h"

iServer::iServer():
m_keepalive(15),
m_ping_timeout(40),
m_connected(false),
m_online(false),
m_me(0),
m_last_udp_ping(0),
m_last_net_packet(0),
m_udp_private_port(0),
m_current_battle(0),
m_relay_host_bot(0)
{
}


iServer::~iServer()
{
	OnDisconnected();
}

void iServer::OnConnected(Socket* sock)
{
	m_connected = true;
	m_online = false;
	m_last_udp_ping = 0;
	m_relay_host_manager_list.clear();
	GetLastPingID() = 0;
	GetPingList().clear();
}

void iServer::OnDisconnected(Socket* sock)
{
    bool connectionwaspresent = m_online || !m_last_denied.lenght() || m_redirecting;
	m_connected = false;
	m_online = false;
    m_redirecting = false;
    m_last_denied = "";
	m_relay_host_manager_list.clear();
	GetLastPingID() = 0;
	GetPingList().clear();
	// delete all users, battles, channels
	m_se.OnDisconnected( connectionwaspresent );
}


void iServer::OnDataReceived( Socket* sock )
{
	if ( !sock ) return;
	m_last_net_packet = time( 0 );
	_OnDataRecieved(sock);
}

bool iServer::IsOnline() const
{
	if ( !m_connected ) return false;
	return m_online;
}

void iServer::RequestSpringUpdate()
{
}


void iServer::TimerUpdate()
{

	if ( !IsConnected() ) return;

	time_t now = time( 0 );

	if ( ( m_last_net_packet > 0 ) && ( ( now - m_last_net_packet ) > m_ping_timeout ) )
	{
		m_se.OnTimeout();
		Disconnect();
		return;
	}

	// joining battle with nat traversal:
	// if we havent finalized joining yet, and udp_reply_timeout seconds has passed since
	// we did UdpPing(our name) , join battle anyway, but with warning message that nat failed.
	// (if we'd receive reply from server, we'd finalize already)
	//
	if (m_last_udp_ping > 0 && (now - m_last_udp_ping) > udp_reply_timeout )
	{
		m_se.OnNATPunchFailed();
	};

	// Is it time for a nat traversal PING?
	if ( ( m_last_udp_ping + m_keepalive ) < now )
	{
		m_last_udp_ping = now;
		// Nat travelsal "ping"
		if ( m_current_battle_id > 0 )
		{
			Battle *battle=GetCurrentBattle();
			if (battle && !battle->GetInGame() )
			{
				if ( battle->GetNatType() == NAT_Hole_punching || battle->GetNatType() == NAT_Fixed_source_ports  )
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
	}

}


void iServer::Ping()
{
	_Ping();
	GetPingList()[GetLastPingID()] = time(0);
}

void iServer::HandlePong( int replyid )
{
	PingList& pinglistcopy = GetPingList();
	PingList::iterator itor = pinglistcopy->find(replyid);
	if ( itor != pinglistcopy->end() )
	{
        m_se->OnPong( (time(0) - itor->second()) );
		pinglistcopy.erase( itor );
    }
}

void iServer::JoinChannel( const std::string& channel, const std::string& key )
{
    m_channel_pw[channel] = key;
	_JoinChannel(channel,key);
}

User* iServer::AcquireRelayhost()
{
	UserVector relaylist = GetRelayHostList();
	unsigned int numbots = relaylist.size();
	if ( numbots > 0 )
	{
		srand ( time(NULL) );
		unsigned int choice = rand() % numbots;
		m_relay_host_manager = relaylist[choice];
		SayPrivate( m_relay_host_manager, "!spawn" );
   }
}

void iServer::OpenBattle( BattleOptions bo )
{
	if ( bo.userelayhost )
	{
		AcquireRelayhost();
	}

	if ( bo.nattype > 0 ) UdpPingTheServer();
	_HostBattle(bo);
}

std::string GenerateScriptPassword()
{
	char buff[8];
	sprintf(&buff,"%04x%04x", rand()&0xFFFF, rand()&0xFFFF);
	return std::string(buff);
}

void iServer::JoinBattle( Battle* battle, const std::string& password )
{
	if (battle)
	{
	if ( battle->GetNatType() == NAT_Hole_punching || battle->GetNatType() == NAT_Fixed_source_ports )
	{
		for (int n=0;n<5;++n) // do 5 udp pings with tiny interval
		{
			UdpPingTheServer( m_user );
			m_last_udp_ping = time(0);
		}
	}
	srand ( time(NULL) );

	_JoinBattle(battle,password,GenerateScriptPassword());
}


void iServer::StartHostedBattle()
{
	if (!m_current_battle) return;
	if (!m_current_battle->IsFounderMe()) return;
	if ( m_current_battle->GetNatType() == NAT_Hole_punching || m_current_battle->GetNatType() == NAT_Fixed_source_ports )
	{
		UdpPingTheServer();
		for (int i=0;i<5;++i)
		{
			UdpPingAllClients();
		}
	}
	_StartHostedBattle();
	m_se->OnStartHostedBattle( m_battle_id );
}

void iServer::LeaveBattle( const Battle* battle)
{
    m_relay_host_bot = 0;
    _LeaveBattle(battle);
}

void iServer::BattleKickPlayer( const Battle* battle, const User* user )
{
	if (!battle) return;
	if (!battle.IsFounderMe()) return;
	if (!battle.IsProxy()) return;
	if (!user) return;
	user->BattleStatus().scriptPassword = GenerateScriptPassword(); // reset user script password, so he can't rejoin
	SetRelayIngamePassword( user );
}


UserVector iServer::GetRelayHostList()
{
	if ( UserExists( "RelayHostManagerList" ) )
	{
		SayPrivate( "RelayHostManagerList", "!listmanagers" );
	}
	UserVector ret;
	for ( unsigned int i = 0; i < m_relay_host_manager_list.count(); i++ )
	{
		User* manager = GetUser( m_relay_host_manager_list[i] );
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

void iServer::SetRelayIngamePassword( const User* user )
{
	if (!user) return;
	if (!m_current_battle) return;
	if ( !m_current_battle->GetInGame() ) return;
	RelayCmd( "SETINGAMEPASSWORD", user->GetNick() + " " + user->BattleStatus().scriptPassword );
}

void iServer::SendScriptToProxy( const std::string& script )
{
	StringVector strings = StringTokenize( script, _T("\n") );
	int relaylenghtprefix = 10 + 1 + m_relay_host_bot.GetNick().lenght() + 2; // SAYPRIVATE + space + botname + space + exclamation mark lenght
	int lenght = script.lenght();
	lenght += relaylenghtprefix + 11 + 1; // CLEANSCRIPT command size
	lenght += strings.GetCount() * ( relaylenghtprefix + 16 + 1 ); // num lines * APPENDSCRIPTLINE + space command size ( \n is already counted in script.size)
	lenght += relaylenghtprefix + 9 + 1; // STARTGAME command size
	int time = lenght / m_sock->GetSendRateLimit(); // calculate time in seconds to upload script
	m_se->OnRelayHostDelayETA(time);
	RelayCmd( "CLEANSCRIPT" );
	for (StringVector::iterator itor; itor != strings->end(); itor++)
	{
		RelayCmd( "APPENDSCRIPTLINE", *itor );
	}
	RelayCmd( "STARTGAME" );
}
