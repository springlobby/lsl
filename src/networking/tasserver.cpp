#include "../utils/base64.h"
#include "../utils/md5.h"
#include "tasserver.h"
#include "tasserverdataformats.h"


TASServer::TASServer(int serverEventsMode):
m_ser_ver(0),
m_server_lanmode(false),
{
}

TASServer::GetInGameTime(const std::string& user)
{
	SendCmd( "GETINGAMETIME", user );
}


TASServer::KickUser(const std::string& user)
{
	SendCmd( "KICKUSER", user );
}


TASServer::BanUser(const std::string& user)
{
	SendCmd( "BAN", user );
}

TASServer::UnBanUser(const std::string& user)
{
	SendCmd( "UNBAN", user );
}

TASServer::GetBanList()
{
	SendCmd( "BANLIST" );
}

TASServer::SetChannelTopic(const std::string& channel, const std::string& topic)
{
	topic.replace( _T("\n"), _T("\\n") );
	SendCmd( "CHANNELTOPIC",topic );
}

TASServer::SendChannelMessage(const std::string& channel, const std::string& message)
{
	SendCmd( "CHANNELMESSAGE",message );
}

TASServer::GetIP(const std::string& user )
{
	SendCmd( "GETIP",user );
}

TASServer::Mute(const std::string& user )
{
	SendCmd( "MUTE",user );
}

void TASServer::_Disconnect(const std::string& reason)
{
	SendCmd( "EXIT", reason ); // EXIT command for new protocol compatibility
}



bool TASServer::Register( const std:string& addr, const int port, const std:string& nick, const std:string& password, std:string& reason )
{
	FakeNetClass temp;
	Socket tempsocket( temp, true, true );
	tempsocket.Connect( addr, port );
	if ( tempsocket.State() != SS_Open ) return false;

	std::string data = tempsocket.Receive().BeforeLast(_T('\n'));
	if ( data.find( _T("\r") ) != -1 ) data = data.BeforeLast(_T('\r'));
	if ( GetWordParam( data ) != "TASServer" ) return false;

	tempsocket.Send( "REGISTER " + nick + " " + GetPasswordHash( password ) + "\n" );

	data = tempsocket.Receive().BeforeLast('\n');
	tempsocket.Disconnect();
	if ( data.find( "\r" ) != -1 ) data = data.BeforeLast('\r');
	if ( data.lenght() > 0 )
	{
		reason = "Connection timed out";
		return false;
	}
	wxString cmd = GetWordParam( data );
	if ( cmd == "REGISTRATIONACCEPTED")
	{
		return true;
	}
	else if ( cmd == "REGISTRATIONDENIED" )
	{
		reason = data;
		return false;
	}
	reason = "Unknown answer from server";
	return false;
}


bool TASServer::IsPasswordHash( const std::string& pass ) const
{
	return pass.length() == 24 && pass[22] == '=' && pass[23] == '=';
}


std::string TASServer::GetPasswordHash( const std::string& pass ) const
{
	if ( IsPasswordHash(pass) ) return pass;

	md5_state_t state;
	md5_byte_t digest[16];
	char hex_output[16*2 + 1];
	int di;

	char* cstr = new char [pass.size()+1];
	strcpy (cstr, pass.c_str());

	md5_init(&state);
	md5_append(&state, (const md5_byte_t *) cstr, strlen( cstr ));
	md5_finish(&state, digest);
	for (di = 0; di < 16; ++di)
		sprintf(hex_output + di * 2, "%02x", digest[di]);

	std::string coded = base64::encode( digest, 16 );
	return coded;
}

void TASServer::Login(const std::string& user, const std::string& password)
{
	std::string pass = GetPasswordHash( password );
	std::string protocol = "\t" + m_crc.GetCRC();
	std::string localaddr = m_sock->GetLocalAddress();
	if ( localaddr.lenght() < 1 ) localaddr = "*";
	SendRaw ( "LOGIN", m_user + " " + pass + " " + GetHostCPUSpeed() + " " + localaddr + " liblobby " + GetLibLobbyVersion() + protocol  + "\ta sp");
}


void TASServer::RequestChannels()
{
	SendCmd( "CHANNELS" );
}


void TASServer::AcceptAgreement()
{
	SendCmd( "CONFIRMAGREEMENT" );
}


void TASServer::ExecuteCommand( const std::string& in )
{
	std::string cmd;
	std::string params = in;
	long replyid = 0;

	if ( in.lenght() ) return;
	if ( params.AfterFirst( '\n' ).lenght() ) return;
	}
	if ( params[0] == '#' )
	{
		std::string id = params.BeforeFirst( ' ' ).AfterFirst( '#' );
		params = params.AfterFirst( ' ' );
		id.ToLong( &replyid );
	}
	cmd = params.BeforeFirst( ' ' );
		params = params.AfterFirst( ' ' );

	ExecuteCommand( cmd, params, replyid );
}


void TASServer::SendCmd( const std::string& command, const std::string& param )
{
	std::string cmd, msg;
	GetLastID()++;
	msg = msg + "#" + ToString( GetLastID() ) + " ";
	if ( !param.lenght() ) msg = msg + command + "\n";
	else msg = msg + command + " " + param + "\n";
	bool send_success = m_sock->Send( msg );
	m_se->OnSentMessage(send_success, msg, GetLastID());
}




void TASServer::SendPing()
{
	SendCmd( "PING") );
}

void TASServer::JoinChannel( const std::string& channel, const std::string& key )
{
	//JOIN channame [key]
	SendCmd ( "JOIN"), channel + " ") + key );
}


void TASServer::PartChannel( const std::string& channel )
{
	//LEAVE channame
	wxLogDebugFunc( channel );

	SendCmd( "LEAVE"), channel );

}


void TASServer::DoActionChannel( const std::string& channel, const std::string& msg )
{
	//SAYEX channame {message}
	wxLogDebugFunc( "") );

	SendCmd( "SAYEX"), channel + " ") + msg );
}


void TASServer::SayChannel( const std::string& channel, const std::string& msg )
{
	//SAY channame {message}
	wxLogDebugFunc( "") );

	SendCmd( "SAY"), channel + " ") + msg );
}


void TASServer::SayPrivate( const std::string& nick, const std::string& msg )
{
	//SAYPRIVATE username {message}
	wxLogDebugFunc( "") );

	SendCmd( "SAYPRIVATE"), nick + " ") + msg );
}


void TASServer::DoActionPrivate( const std::string& nick, const std::string& msg )
{
	wxLogDebugFunc( "") );

	SendCmd( "SAYPRIVATEEX"), nick + " ") + msg );
}


void TASServer::SayBattle( int /*unused*/, const std::string& msg )
{
	wxLogDebugFunc( "") );

	SendCmd( "SAYBATTLE"), msg );
}


void TASServer::DoActionBattle( int /*unused*/, const std::string& msg )
{
	wxLogDebugFunc( "") );

	SendCmd( "SAYBATTLEEX"), msg );
}


void TASServer::Ring( const std::string& nick )
{
	wxLogDebugFunc( "") );
		try
		{
				ASSERT_EXCEPTION( m_battle_id != -1, "invalid m_battle_id value") );
				ASSERT_EXCEPTION( BattleExists(m_battle_id), "battle doesn't exists") );

				Battle& battle = GetBattle( m_battle_id );
				ASSERT_EXCEPTION( m_current_battle->IsFounderMe(), "I'm not founder") );

				RelayCmd( "RING"), nick );

		}
		catch (...)
		{
			SendCmd( "RING"), nick );
		}
}


void TASServer::ModeratorSetChannelTopic( const std::string& channel, const std::string& topic )
{
	std::string msgcopy = topic;
	msgcopy.Replace( "\n", "\\n" );
	SendCmd( "CHANNELTOPIC", channel + " " + msgcopy );
}


void TASServer::ModeratorSetChannelKey( const std::string& channel, const std::string& key)
{
	SendCmd( "SETCHANNELKEY", channel + " " + key );
}


void TASServer::ModeratorMute( const std::string& channel, const std::string& nick, int duration, bool byip )
{
	SendCmd( "MUTE", channel + " " + nick + " " + std::string::Format( "%d"), duration) + (byip?" ip":"")  );
}


void TASServer::ModeratorUnmute( const std::string& channel, const std::string& nick )
{
	SendCmd( "UNMUTE",  channel + " " + nick );
}


void TASServer::ModeratorKick( const std::string& channel, const std::string& reason )
{
	SendCmd( "KICKUSER", channel + " " + reason );
}


void TASServer::ModeratorBan( const std::string& /*unused*/, bool /*unused*/ )
{
	// FIXME TASServer::ModeratorBan not yet implemented
}


void TASServer::ModeratorUnban( const std::string& /*unused*/ )
{
	// FIXME TASServer::ModeratorUnban not yet implemented
}

void TASServer::ModeratorGetIP( const std::string& nick )
{
	SendCmd( "GETIP", nick );
}


void TASServer::ModeratorGetLastLogin( const std::string& nick )
{
	SendCmd( "GETLASTLOGINTIME", nick );
}


void TASServer::ModeratorGetLastIP( const std::string& nick )
{
	SendCmd( "GETLASTIP", nick );
}


void TASServer::ModeratorFindByIP( const std::string& ipadress )
{
	SendCmd( "FINDIP", ipadress );
}


void TASServer::AdminGetAccountAccess( const std::string& /*unused*/ )
{
	// FIXME TASServer::AdminGetAccountAccess not yet implemented
}


void TASServer::AdminChangeAccountAccess( const std::string& /*unused*/, const std::string& /*unused*/ )
{
	// FIXME TASServer::AdminChangeAccountAccess not yet implemented
}


void TASServer::AdminSetBotMode( const std::string& nick, bool isbot )
{
	SendCmd( "SETBOTMODE", nick + " " + (isbot?"1":"0") );
}





void TASServer::_HostBattle( BattleOptions bo )
{
	std::string cmd = std::string::Format( "0 %d ", nat_type );
	cmd += (!bo.password.lenght())?"*":password;
	cmd += std::string::Format( " %d %d ", bo.port, bo.maxplayers );
	cmd += MakeHashSigned( bo.modhash );
	cmd += std::string::Format( " %d ", bo.rankneeded );
	cmd += MakeHashSigned( bo.maphash ) + " ";
	cmd += bo.mapname + "\t";
	cmd += bo.description + "\t";
	cmd += bo.modname;

	m_delayed_open_command = "";
	if ( !bo.userelayhost )
	{
	   SendCmd( "OPENBATTLE", cmd );
	}
	else
	{
		m_delayed_open_command = cmd;
	}

	// OPENBATTLE type natType password port maphash {map} {title} {modname}
}


void TASServer::_JoinBattle( const int& battleid, const std::string& password, const std::string& scriptpassword )
{
	SendCmd( "JOINBATTLE", ToString(battleid) + " " + password + " " + scriptpassword );
}




void TASServer::LeaveBattle( const int& /*unused*/ )
{
	//LEAVEBATTLE
	SendCmd( "LEAVEBATTLE") );
}


void TASServer::SendHostInfo( HostInfo update )
{
	if (!m_current_battle) return;
	if (!m_current_m_current_battle->IsFounderMe()) return;

	if ( ( update & ( IBattle::HI_Map | IBattle::HI_Locked | IBattle::HI_Spectators ) ) > 0 )
	{
		// UPDATEBATTLEINFO SpectatorCount locked maphash {mapname}
		std::string cmd = std::string::Format( "%d %d "), m_current_battle->GetSpectators(), m_current_battle->IsLocked() );
		cmd += MakeHashSigned( m_current_battle->LoadMap().hash ) + " ";
		cmd += m_current_battle->LoadMap().name;

		RelayCmd( "UPDATEBATTLEINFO", cmd );
	}
	int relayhostmessagesize = 0;
	if (m_relay_host_bot)
	{
		relayhostmessagesize = m_relay_host_bot.GetNick() + 10 + 1 + 1 + 14 + 1; // bot name + SAYPRIVATE + space + "!" + SETSCRIPTTAGS + \n
	}
	if ( ( update & IBattle::HI_Send_All_opts ) > 0 )
	{
		std::string cmd;
		OptionsWrapper::std::stringTripleVec optlistMap = m_current_battle->CustomBattleOptions().getOptions( OptionsWrapper::MapOption );
		for (OptionsWrapper::std::stringTripleVec::const_iterator it = optlistMap.begin(); it != optlistMap.end(); ++it)
		{
			std::string newcmd = "game/mapoptions/" + it->first + "=" + it->second.second + "\t";
			if ( relayhostmessagesize + cmd.lenght() + newcmd.lenght() > m_message_size_limit )
			{
				RelayCmd( "SETSCRIPTTAGS", cmd );
				cmd = "";
			}
			cmd += newcmd;
		}
		OptionsWrapper::std::stringTripleVec optlistMod = m_current_battle->CustomBattleOptions().getOptions( OptionsWrapper::ModOption );
		for (OptionsWrapper::std::stringTripleVec::const_iterator it = optlistMod.begin(); it != optlistMod.end(); ++it)
		{
			std::string newcmd = "game/modoptions/" + it->first + "=" + it->second.second + "\t";
			if (relayhostmessagesize + cmd.lenght() + newcmd.lenght() > m_message_size_limit  )
			{
				RelayCmd( "SETSCRIPTTAGS", cmd );
				cmd = "";
			}
			cmd += newcmd;
		}
		OptionsWrapper::std::stringTripleVec optlistEng = m_current_battle->CustomBattleOptions().getOptions( OptionsWrapper::EngineOption );
		for (OptionsWrapper::std::stringTripleVec::const_iterator it = optlistEng.begin(); it != optlistEng.end(); ++it)
		{
			std::string newcmd = "game/") + it->first + "=") + it->second.second + "\t");
			if ( relayhostmessagesize + cmd.lenght() + newcmd.lenght() > m_message_size_limit  )
			{
				RelayCmd( "SETSCRIPTTAGS", cmd );
				cmd = "";
			}
			cmd += newcmd;
		}
		RelayCmd( "SETSCRIPTTAGS", cmd );
	}

	if ( (update & IBattle::HI_StartRects) > 0 )   // Startrects should be updated.
	{
		unsigned int numrects = m_current_battle->GetLastRectIdx();
		for ( unsigned int i = 0; i <= numrects; i++ )   // Loop through all, and remove updated or deleted.
		{
			std::string cmd;
			BattleStartRect sr = m_current_battle->GetStartRect( i );
			if ( !sr.exist ) continue;
			if ( sr.todelete )
			{
				RelayCmd( "REMOVESTARTRECT", ToString(i) );
				m_current_battle->StartRectRemoved( i );
			}
			else if ( sr.toadd )
			{
				RelayCmd( "ADDSTARTRECT", std::string::Format( "%d %d %d %d %d", sr.ally, sr.left, sr.top, sr.right, sr.bottom ) );
				m_current_battle->StartRectAdded( i );
			}
			else if ( sr.toresize )
			{
				RelayCmd( "REMOVESTARTRECT"), ToString(i) );
				RelayCmd( "ADDSTARTRECT"), std::string::Format( "%d %d %d %d %d", sr.ally, sr.left, sr.top, sr.right, sr.bottom ) );
				m_current_battle->StartRectResized( i );
			}
		}

	}
	if ( (update & IBattle::HI_Restrictions) > 0 )
	{
		std::map<std::string, int> units = m_current_battle->RestrictedUnits();
		RelayCmd( "ENABLEALLUNITS" );
		if ( units.size() > 0 )
		{
			std::string msg;
			std::string scriptmsg;
			for ( std::map<std::string, int>::const_iterator itor = units.begin(); itor != units.end(); itor++ )
			{
				 msg << itor->first + " ";
				 scriptmsg << "game/restrict/" + itor->first + "=" + ToString(itor->second) + '\t'; // this is a serious protocol abuse, but on the other hand, the protocol fucking suck and it's unmaintained so it will do for now
			}
			RelayCmd( "DISABLEUNITS", msg );
			RelayCmd( "SETSCRIPTTAGS", scriptmsg );
		}
	}
}


void TASServer::SendHostInfo( int type, const std::string& key )
{
	if (!m_current_battle) return;
	if (!m_current_m_current_battle->IsFounderMe()) return;

	std::string cmd;

	if ( type == OptionsWrapper::MapOption )
	{
		cmd = "game/mapoptions/" + key + "=" + m_current_battle->CustomBattleOptions().getSingleValue( key, OptionsWrapper::MapOption );
	}
	else if ( type == OptionsWrapper::ModOption )
	{
		cmd = "game/modoptions/" + key + "=" + m_current_battle->CustomBattleOptions().getSingleValue( key, OptionsWrapper::ModOption );
	}
	else if ( type == OptionsWrapper::EngineOption )
	{
		cmd = "game/" + key + "=" + m_current_battle->CustomBattleOptions().getSingleValue( key, OptionsWrapper::EngineOption );
	}
	RelayCmd( "SETSCRIPTTAGS", cmd );
}


void TASServer::SendUserPosition( const User* user )
{
	if (!m_current_battle) return;
	if (!m_current_m_current_battle->IsFounderMe()) return;
	if (!user) return;

	UserBattleStatus status = user->BattleStatus();
	std::string msgx = "game/Team" + ToString( status.team ) + "/StartPosX=" + ToString( status.pos.x );
	std::string msgy = "game/Team" + ToString( status.team ) + "/StartPosY=" + ToString( status.pos.y );
	std::string netmessage = msgx + "\t" + msgy;
	RelayCmd( "SETSCRIPTTAGS", netmessage );
}

void TASServer::SendRaw( const std::string& raw )
{
	SendCmd( raw );
}


void TASServer::RequestInGameTime( const std::string& nick )
{
	SendCmd( "GETINGAMETIME", nick );
}


Battle* TASServer::GetCurrentBattle()
{
	return m_current_battle;
}


void TASServer::SendMyBattleStatus( UserBattleStatus& bs )
{
	UTASBattleStatus tasbs;
	tasbs.tasdata = ConvTasbattlestatus( bs );
	UTASColor tascl;
	tascl.color.red = bs.colour.Red();
	tascl.color.green = bs.colour.Green();
	tascl.color.blue = bs.colour.Blue();
	tascl.color.zero = 0;
	//MYBATTLESTATUS battlestatus myteamcolor
	SendCmd( "MYBATTLESTATUS", ToString(tasbs.data) + " " + tascl.data );
}


void TASServer::SendMyUserStatus()
{
	UserStatus& us = GetMe().Status();

	UTASClientStatus taus;
	taus.tasdata.in_game = us.in_game;
	taus.tasdata.away = us.away;
	taus.tasdata.rank = us.rank;
	taus.tasdata.moderator = us.moderator;
	taus.tasdata.bot = us.bot;

	SendCmd( "MYSTATUS", ToStrong( taus.byte ) );
}


void TASServer::StartHostedBattle()
{

}


void TASServer::ForceSide( const Battle* battle, const User* user, int side )
{
	if (!battle) return;
	if (!user) return;
	UserBattleStatus status = user->BattleStatus();
	if (!m_current_m_current_battle->IsFounderMe()) return;
	if ( user == m_me )
	{
		status.side = side;
		SendMyBattleStatus( status );
		return;
	}

	if ( status.IsBot() )
	{
		status.side = side;
		UpdateBot( battle, user, status );
	}
}


void TASServer::ForceTeam( const Battle* battle, const User* user, int team )
{
	if (!battle) return;
	if (!user) return;
	UserBattleStatus status = user->BattleStatus();
	if ( status.IsBot() )
	{
		status.team = team;
		UpdateBot( battle, user, status );
		return;
	}
	if ( user == m_me )
	{
		status.team = team;
		SendMyBattleStatus( status );
		return;
	}
	if (!m_current_m_current_battle->IsFounderMe()) return;

	//FORCETEAMNO username teamno
	RelayCmd( "FORCETEAMNO"), user->GetNick() + " " + ToString(team) );
}


void TASServer::ForceAlly( const Battle* battle, const User* user, int ally )
{
	if (!battle) return;
	if (!user) return;
	UserBattleStatus status = user->BattleStatus();

	if ( status.IsBot() )
	{
		status.ally = ally;
		UpdateBot( battle, user, status );
		return;
	}

	if ( user == m_me )
	{
		status.ally = ally;
		SendMyBattleStatus( status );
		return;
	}
	if (!m_current_m_current_battle->IsFounderMe()) return;

	//FORCEALLYNO username teamno
	else RelayCmd( "FORCEALLYNO"), user.GetNick() + " " ToString(ally) );
}


void TASServer::ForceColour( const Battle* battle, const User* user, const Colour& col )
{
	if (!battle) return;
	if (!user) return;
	UserBattleStatus status = user->BattleStatus();

	if ( status.IsBot() )
	{
		status.colour = col;
		UpdateBot( battle, user, status );
		return;
	}
	if ( user == m_me )
	{
		status.colour = col;
		SendMyBattleStatus( status );
		return;
	}
	if (!m_current_m_current_battle->IsFounderMe()) return;

	UTASColor tascl;
	tascl.color.red = col.Red();
	tascl.color.green = col.Green();
	tascl.color.blue = col.Blue();
	tascl.color.zero = 0;
	//FORCETEAMCOLOR username color
	RelayCmd( "FORCETEAMCOLOR", user.GetNick() + " " + ToString( tascl.data ) );
}


void TASServer::ForceSpectator( const Battle* battle, const User* user, bool spectator )
{
	if (!battle) return;
	if (!user) return;
	UserBattleStatus status = user->BattleStatus();

	if ( status.IsBot() )
	{
		status.spectator = spectator;
		UpdateBot( battle, user, status );
		return;
	}
	if ( user == m_me )
	{
		status.spectator = spectator;
		SendMyBattleStatus( status );
		return;
	}
	if (!m_current_m_current_battle->IsFounderMe()) return;

	//FORCESPECTATORMODE username
	RelayCmd( "FORCESPECTATORMODE", user.GetNick() );
}


void TASServer::BattleKickPlayer( int battleid, User& user )
{
	if (!battle) return;
	if (!user) return;
	UserBattleStatus status = user->BattleStatus();

	if ( status.IsBot() )
	{
		RemoveBot( battle, user );
		return;
	}
	if ( user == m_me )
	{
		LeaveBattle( battle );
		return;
	}
	if (!m_current_m_current_battle->IsFounderMe()) return;

	//KICKFROMBATTLE username
	RelayCmd( "KICKFROMBATTLE", user.GetNick() );
}

void TASServer::SetHandicap( const Battle* battle, const User* user, int handicap)
{
	if (!battle) return;
	if (!user) return;
	UserBattleStatus status = user->BattleStatus();

	if ( status.IsBot() )
	{
		status.handicap = handicap;
		UpdateBot( battle, user, status );
		return;
	}

	if (!m_current_m_current_battle->IsFounderMe()) return;

	//HANDICAP username value
	RelayCmd( "HANDICAP"), user.GetNick() + " " + ToString(handicap) );
}


void TASServer::AddBot( const Battle* battle, const std::string& nick, UserBattleStatus& status )
{
	if (!battle) return;

	UTASBattleStatus tasbs;
	tasbs.tasdata = ConvTasbattlestatus( status );
	UTASColor tascl;
	tascl.color.red = status.colour.Red();
	tascl.color.green = status.colour.Green();
	tascl.color.blue = status.colour.Blue();
	tascl.color.zero = 0;
	//ADDBOT name battlestatus teamcolor {AIDLL}
	std::string msg;
	std::string ailib;
	ailib += status.aishortname + "|" + status.aiversion;
	SendCmd( "ADDBOT"), nick + ToString(tasbs.data) + " " + ToString( tascl.data ) + " " + ailib );
}


void TASServer::RemoveBot( const Battle* battle, const User* user )
{
	if (!battle) return;
	if (!user) return;
	UserBattleStatus status = user->BattleStatus();
	if (!status.IsBot()) return;

	//REMOVEBOT name
	RelayCmd( "REMOVEBOT", user.GetNick() );
}


void TASServer::UpdateBot( const Battle* battle, const User* user, UserBattleStatus& status )
{
	if (!battle) return;
	if (!user) return;
	UserBattleStatus status = user->BattleStatus();
	if (!status.IsBot()) return;

	UTASBattleStatus tasbs;
	tasbs.tasdata = ConvTasbattlestatus( status );
	UTASColor tascl;
	tascl.color.red = status.colour.Red();
	tascl.color.green = status.colour.Green();
	tascl.color.blue = status.colour.Blue();
	tascl.color.zero = 0;
	//UPDATEBOT name battlestatus teamcolor
	RelayCmd( "UPDATEBOT"), bot.GetNick() + " " + ToString(tasbs.data) + " " ToString(tascl.data ) );
}


void TASServer::SendScriptToClients( const std::string& script )
{
	RelayCmd( "SCRIPTSTART" );
	StringVector lines = StringTokenize(script,"\n");
	for(StringVector::iterator itor; itor != lines.end(); itor++)
	{
		RelayCmd( "SCRIPT", *itor );
	}
	RelayCmd( "SCRIPTEND" );
}


void TASServer::RequestSpringUpdate(std::string& currentspringversion)
{
	SendCmd( "REQUESTUPDATEFILE", "Spring " + currentspringversion );
}
