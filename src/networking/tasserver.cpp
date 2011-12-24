#include "../utils/base64.h"
#include "../utils/md5.h"
#include "tasserver.h"
#include "tasserverdataformats.h"


TASServer::TASServer(int serverEventsMode):
m_ser_ver(0),
m_server_lanmode(false),
{
}

void TASServer::GetInGameTime(const std::string& user)
{
	SendCmd( "GETINGAMETIME", user );
}


void TASServer::KickUser(const std::string& user)
{
	SendCmd( "KICKUSER", user );
}


void TASServer::BanUser(const std::string& user)
{
	SendCmd( "BAN", user );
}

void TASServer::UnBanUser(const std::string& user)
{
	SendCmd( "UNBAN", user );
}

void TASServer::GetBanList()
{
	SendCmd( "BANLIST" );
}

void TASServer::SetChannelTopic(const std::string& channel, const std::string& topic)
{
	topic.replace( _T("\n"), _T("\\n") );
	SendCmd( "CHANNELTOPIC",topic );
}

void TASServer::SendChannelMessage(const std::string& channel, const std::string& message)
{
	SendCmd( "CHANNELMESSAGE",message );
}

void TASServer::GetIP(const std::string& user )
{
	SendCmd( "GETIP",user );
}

void TASServer::GetChannelMutelist(const std::string& channel )
{
	SendCmd( "MUTELIST",channel );
}

void TASServer::ChangePassword(const std::string& oldpassword, const std::string& newpassword )
{
	SendCmd( "CHANGEPASSWORD",GetPasswordHash(oldpassword) + " " + GetPasswordHash(newpassword) );
}

void TASServer::GetMD5(const std::string& text, const std::string& newpassword )
{
	return GetPasswordHash(params)
}

void TASServer::Rename(const std::string& newnick)
{
	SendCmd( "RENAMEACCOUNT", newnick );
}

void TASServer::_Disconnect(const std::string& reason)
{
	SendCmd( "EXIT", reason ); // EXIT command for new protocol compatibility
}


void TASServer::GetLastLoginTime(const std::string& user)
{
	SendCmd( "GETLASTLOGINTIME", user );
}

void TASServer::GetUserIP(const std::string& user)
{
	SendCmd( "FINDIP", user );
}

void TASServer::GetLastUserIP(const std::string& user)
{
	SendCmd( "GETLASTIP", user );
}


int TASServer::Register( const std:string& addr, const int port, const std:string& nick, const std:string& password )
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
		return 1;
	}
	wxString cmd = GetWordParam( data );
	if ( cmd == "REGISTRATIONACCEPTED")
	{
		return 0;
	}
	else if ( cmd == "REGISTRATIONDENIED" )
	{
		return 2;
	}
	return 3;
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



////////////////////////////////////////////////////////////////////////////////
////                          parse & execute section                      /////
////////////////////////////////////////////////////////////////////////////////




void TASServer::ExecuteCommand( const std::string& cmd, const std::string& inparams, int replyid )
{
	std::string params = inparams;
	int pos, cpu, id, nat, port, maxplayers, rank, specs, units, top, left, right, bottom, ally, type;
	bool haspass,lanmode = false;
	std::string hash;
	std::string nick, contry, host, map, title, mod, channel, error, msg, owner, ai, supported_spring_version, topic;
	//NatType ntype;
	UserStatus cstatus;
	UTASClientStatus tasstatus;
	UTASBattleStatus tasbstatus;
	UserBattleStatus bstatus;
	UTASColor color;

	if ( cmd == "TASSERVER")
	{
		mod = GetWordParam( params );
		m_ser_ver = atoi( mod )
		supported_spring_version = GetWordParam( params );
		m_nat_helper_port = GetIntParam( params );
		lanmode = GetBoolParam( params );
		m_server_lanmode = lanmode;
		m_se->OnConnected( m_server_name, mod, (m_ser_ver > 0), supported_spring_version, lanmode );
	}
	else if ( cmd == "ACCEPTED" )
	{
		if ( m_online ) return; // in case is the server sends WTF
		m_online = true;
		m_user = params;
		m_ping_thread = new PingThread( *this, 10000 );
		m_ping_thread->Init();
		m_se->OnLogin( );
	}
	else if ( cmd == "MOTD" )
	{
		m_se->OnMotd( params );
	}
	else if ( cmd == "ADDUSER" )
	{
		nick = GetWordParam( params );
		contry = GetWordParam( params );
		cpu = GetIntParam( params );
		if ( params.IsEmpty() )
		{
			// if server didn't send any account id to us, fill with an always increasing number
			id = m_account_id_count;
			m_account_id_count++;
		}
		else
		{
			id = GetIntParam( params );
		}
		m_se->OnNewUser( nick, contry, cpu, ToString(id) );
		if ( nick == m_relay_host_bot )
		{
		   RelayCmd( "OPENBATTLE", m_delayed_open_command ); // relay bot is deployed, send host command
		   m_delayed_open_command = "";
		}
	}
	else if ( cmd == "CLIENTSTATUS") )
	{
		nick = GetWordParam( params );
		tasstatus.byte = GetIntParam( params );
		cstatus = ConvTasclientstatus( tasstatus.tasdata );
		m_se->OnUserStatus( nick, cstatus );
	}
	else if ( cmd == "BATTLEOPENED") )
	{
		id = GetIntParam( params );
		type = GetIntParam( params );
		nat = GetIntParam( params );
		nick = GetWordParam( params );
		host = GetWordParam( params );
		port = GetIntParam( params );
		maxplayers = GetIntParam( params );
		haspass = GetBoolParam( params );
		rank = GetIntParam( params );
		hash = MakeHashUnsigned( GetWordParam( params ) );
		map = GetSentenceParam( params );
		title = GetSentenceParam( params );
		mod = GetSentenceParam( params );
		m_se->OnBattleOpened( id, (BattleType)type, IntToNatType( nat ), nick, host, port, maxplayers,
							  haspass, rank, hash, map, title, mod );
		if ( nick == m_relay_host_bot )
		{
		   GetBattle( id ).SetProxy( m_relay_host_bot );
		   JoinBattle( id, sett().GetLastHostPassword() ); // autojoin relayed host battles
		}
	}
	else if ( cmd == "JOINEDBATTLE") )
	{
		id = GetIntParam( params );
		nick = GetWordParam( params );
		std::string userScriptPassword = GetWordParam( params );
		m_se->OnUserJoinedBattle( id, nick, userScriptPassword );
	}
	else if ( cmd == "UPDATEBATTLEINFO") )
	{
		id = GetIntParam( params );
		specs = GetIntParam( params );
		haspass = GetBoolParam( params );
		hash = MakeHashUnsigned( GetWordParam( params ) );
		map = GetSentenceParam( params );
		m_se->OnBattleInfoUpdated( id, specs, haspass, hash, map );
	}
	else if ( cmd == "LOGININFOEND") )
	{
		if ( sett().GetReportStats() )
		{
			std::string version = GetSpringLobbyVersion(false);
			std::string aux = GetSpringLobbyAuxVersion();
			aux.Replace( " "), "") );
			aux = " ") + aux;
			std::string os = wxPlatformInfo::Get().GetOperatingSystemIdName();
			os.Replace( " "), "") );
			std::string wxversion = wxVERSION_STRING;
			wxversion.Replace( " "), "") );
			std::string reportstring = "stats.report ") + version + " ") + wxversion + " ") + os + aux;
			if ( UserExists( "SL_bot") ) ) SayPrivate( "SL_bot"), reportstring );
		}
		if ( UserExists( "RelayHostManagerList") ) ) SayPrivate( "RelayHostManagerList"), "!lm") );
		m_se->OnLoginInfoComplete();
	}
	else if ( cmd == "REMOVEUSER") )
	{
		nick = GetWordParam( params );
		if ( nick == m_user ) return; // to prevent peet doing nasty stuff to you, watch your back!
		m_se->OnUserQuit( nick );
	}
	else if ( cmd == "BATTLECLOSED") )
	{
		id = GetIntParam( params );
		if ( m_battle_id == id ) m_relay_host_bot = "");
		m_se->OnBattleClosed( id );
	}
	else if ( cmd == "LEFTBATTLE") )
	{
		id = GetIntParam( params );
		nick = GetWordParam( params );
		m_se->OnUserLeftBattle( id, nick );
	}
	else if ( cmd == "PONG") )
	{
		HandlePong( replyid );
	}
	else if ( cmd == "JOIN") )
	{
		channel = GetWordParam( params );
		m_se->OnJoinChannelResult( true, channel, "") );
	}
	else if ( cmd == "JOIN") )
	{
		channel = GetWordParam( params );
		error = GetSentenceParam( params );
		m_se->OnJoinChannelResult( false, channel, error );
	}
	else if ( cmd == "SAID") )
	{
		channel = GetWordParam( params );
		nick = GetWordParam( params );
		m_se->OnChannelSaid( channel, nick, params );
	}
	else if ( cmd == "JOINED") )
	{
		channel = GetWordParam( params );
		nick = GetWordParam( params );
		m_se->OnUserJoinChannel( channel, nick );
	}
	else if ( cmd == "LEFT") )
	{
		channel = GetWordParam( params );
		nick = GetWordParam( params );
		msg = GetSentenceParam( params );
		m_se->OnChannelPart( channel, nick, msg );
	}
	else if ( cmd == "CHANNELTOPIC") )
	{
		channel = GetWordParam( params );
		nick = GetWordParam( params );
		pos = GetIntParam( params );
		params.Replace( "\\n"), "\n") );
		m_se->OnChannelTopic( channel, nick, params, pos/1000 );
	}
	else if ( cmd == "SAIDEX") )
	{
		channel = GetWordParam( params );
		nick = GetWordParam( params );
		m_se->OnChannelAction( channel, nick, params );
	}
	else if ( cmd == "CLIENTS") )
	{
		channel = GetWordParam( params );
		while ( (nick = GetWordParam( params )) != "") )
		{
			m_se->OnChannelJoin( channel, nick );
		}
	}
	else if ( cmd == "SAYPRIVATE") )
	{
		nick = GetWordParam( params );
		if ( ( ( nick == m_relay_host_bot ) || ( nick == m_relay_host_manager ) ) && params.StartsWith( "!") ) ) return; // drop the message
		if ( ( nick == "RelayHostManagerList") ) && ( params == "!lm") ) ) return;// drop the message
		if ( nick == "SL_bot") )
		{
			if ( params.StartsWith( "stats.report") ) ) return;
		}
		m_se->OnPrivateMessage( nick, params, true );
	}
	else if ( cmd == "SAYPRIVATEEX") )
	{
		nick = GetWordParam( params );
		m_se->OnPrivateMessageEx( nick, params, true );
	}
	else if ( cmd == "SAIDPRIVATE") )
	{
		nick = GetWordParam( params );
		if ( nick == m_relay_host_bot )
		{
			if ( params.StartsWith("JOINEDBATTLE")) )
			{
				GetWordParam( params ); // skip first word, it's the message itself
				id = GetIntParam( params );
				std::string usernick = GetWordParam( params );
				std::string userScriptPassword = GetWordParam( params );
				try
				{
					User& usr = GetUser(usernick);
					usr.BattleStatus().scriptPassword = userScriptPassword;
					Battle* battle = GetCurrentBattle();
					if (battle)
					{
						if ( battle->CheckBan( usr ) ) return;
					}
					SetRelayIngamePassword( usr );
				} catch (...) {}
				return;
			}
		}
		if ( nick == m_relay_host_manager )
		{
		  if ( params.StartsWith( "\001") ) ) // error code
		  {
			m_se->OnServerMessageBox( params.AfterFirst( ' ') ) );
		  }
		  else
		  {
			m_relay_host_bot = params;
		  }
		  m_relay_host_manager = "");
		  return;
		}
		if ( nick == "RelayHostManagerList") )
				{
					if  ( params.StartsWith("list ")) )
					{
						 std::string list = params.AfterFirst( ' ') );
						 m_relay_host_manager_list = std::stringTokenize( list, "\t") );
						 return;
					}
				}
		m_se->OnPrivateMessage( nick, params, false );
	}
	else if ( cmd == "SAIDPRIVATEEX") )
	{
		nick = GetWordParam( params );
		m_se->OnPrivateMessageEx( nick, params, false );
	}
	else if ( cmd == "JOINBATTLE") )
	{
		id = GetIntParam( params );
		hash = MakeHashUnsigned( GetWordParam( params ) );
		m_battle_id = id;
		m_se->OnJoinedBattle( id, hash );
		m_se->OnBattleInfoUpdated( m_battle_id );
		try
		{
		 if (GetBattle(id).IsProxy()) RelayCmd("SUPPORTSCRIPTPASSWORD")); // send flag to relayhost marking we support script passwords
		} catch(...) {}
	}
	else if ( cmd == "CLIENTBATTLESTATUS") )
	{
		nick = GetWordParam( params );
		tasbstatus.data = GetIntParam( params );
		bstatus = ConvTasbattlestatus( tasbstatus.tasdata );
		color.data = GetIntParam( params );
		bstatus.colour = wxColour( color.color.red, color.color.green, color.color.blue );
		m_se->OnClientBattleStatus( m_battle_id, nick, bstatus );
	}
	else if ( cmd == "ADDSTARTRECT") )
	{
		//ADDSTARTRECT allyno left top right bottom
		ally = GetIntParam( params );
		left = GetIntParam( params );
		top = GetIntParam( params );
		right = GetIntParam( params );
		bottom = GetIntParam( params );;
		m_se->OnBattleStartRectAdd( m_battle_id, ally, left, top, right, bottom );
	}
	else if ( cmd == "REMOVESTARTRECT") )
	{
		//REMOVESTARTRECT allyno
		ally = GetIntParam( params );
		m_se->OnBattleStartRectRemove( m_battle_id, ally );
	}
	else if ( cmd == "ENABLEALLUNITS") )
	{
		//"ENABLEALLUNITS" params: "".
		m_se->OnBattleEnableAllUnits( m_battle_id );
	}
	else if ( cmd == "ENABLEUNITS") )
	{
		//ENABLEUNITS unitname1 unitname2
		while ( (nick = GetWordParam( params )) != "") )
		{
			m_se->OnBattleEnableUnit( m_battle_id, nick );
		}
	}
	else if ( cmd == "DISABLEUNITS") )
	{
		//"DISABLEUNITS" params: "arm_advanced_radar_tower arm_advanced_sonar_station arm_advanced_torpedo_launcher arm_dragons_teeth arm_energy_storage arm_eraser arm_fark arm_fart_mine arm_fibber arm_geothermal_powerplant arm_guardian"
		while ( (nick = GetWordParam( params )) != "") )
		{
			m_se->OnBattleDisableUnit( m_battle_id, nick );
		}
	}
	else if ( cmd == "CHANNEL") )
	{
		channel = GetWordParam( params );
		units = GetIntParam( params );
		topic = GetSentenceParam( params );
		m_se->OnChannelList( channel, units, topic );
	}
	else if ( cmd == "ENDOFCHANNELS") )
	{
		//Cmd: ENDOFCHANNELS params:
	}
	else if ( cmd == "REQUESTBATTLESTATUS") )
	{
		m_se->OnRequestBattleStatus( m_battle_id );
	}
	else if ( cmd == "SAIDBATTLE") )
	{
		nick = GetWordParam( params );
		m_se->OnSaidBattle( m_battle_id, nick, params );
	}
	else if ( cmd == "SAIDBATTLEEX") )
	{
		nick = GetWordParam( params );
		m_se->OnBattleAction( m_battle_id, nick, params );
	}
	else if ( cmd == "AGREEMENT") )
	{
		msg = GetSentenceParam( params );
		m_agreement += msg + "\n");
	}
	else if ( cmd == "AGREEMENTEND") )
	{
		m_se->OnAcceptAgreement( m_agreement );
		m_agreement = "");
	}
	else if ( cmd == "OPENBATTLE") )
	{
		m_battle_id = GetIntParam( params );
		m_se->OnHostedBattle( m_battle_id );
	}
	else if ( cmd == "ADDBOT") )
	{
		// ADDBOT BATTLE_ID name owner battlestatus teamcolor {AIDLL}
		id = GetIntParam( params );
		nick = GetWordParam( params );
		owner = GetWordParam( params );
		tasbstatus.data = GetIntParam( params );
		bstatus = ConvTasbattlestatus( tasbstatus.tasdata );
		color.data = GetIntParam( params );
		bstatus.colour = wxColour( color.color.red, color.color.green, color.color.blue );
		ai = GetSentenceParam( params );
		if ( ai.IsEmpty() ) {
			wxLogWarning( std::string::Format( "Recieved illegal ADDBOT (empty dll field) from %s for battle %d"), nick.c_str(), id ) );
			ai = "INVALID|INVALID");
		}
		if( usync().VersionSupports( SpringUnitSync::USYNC_GetSkirmishAI ) )
		{
			if (ai.Find('|')) != -1)
			{
				bstatus.aiversion = ai.AfterLast( '|') );
				ai = ai.BeforeLast( '|') );
			}
			bstatus.aishortname = ai;
		}
		else
		{
			 bstatus.aishortname = ai;
		}
		bstatus.owner =owner;
		m_se->OnBattleAddBot( id, nick, bstatus );
	}
	else if ( cmd == "UPDATEBOT") )
	{
		id = GetIntParam( params );
		nick = GetWordParam( params );
		tasbstatus.data = GetIntParam( params );
		bstatus = ConvTasbattlestatus( tasbstatus.tasdata );
		color.data = GetIntParam( params );
		bstatus.colour = wxColour( color.color.red, color.color.green, color.color.blue );
		m_se->OnBattleUpdateBot( id, nick, bstatus );
		//UPDATEBOT BATTLE_ID name battlestatus teamcolor
	}
	else if ( cmd == "REMOVEBOT") )
	{
		id = GetIntParam( params );
		nick = GetWordParam( params );
		m_se->OnBattleRemoveBot( id, nick );
		//REMOVEBOT BATTLE_ID name
	}
	else if ( cmd == "RING") )
	{
		nick = GetWordParam( params );
		m_se->OnRing( nick );
		//RING username
	}
	else if ( cmd == "SERVERMSG") )
	{
				m_se->OnServerMessage( params );
		//SERVERMSG {message}
	}
	else if ( cmd == "JOINBATTLEFAILED") )
	{
		msg = GetSentenceParam( params );
		m_se->OnServerMessage( "Failed to join battle. ") + msg );
		//JOINBATTLEFAILED {reason}
	}
	else if ( cmd == "OPENBATTLEFAILED") )
	{
		msg = GetSentenceParam( params );
		m_se->OnServerMessage( "Failed to host new battle on server. ") + msg );
		//OPENBATTLEFAILED {reason}
	}
	else if ( cmd == "JOINFAILED") )
	{
		channel = GetWordParam( params );
		msg = GetSentenceParam( params );
		m_se->OnServerMessage( "Failed to join channel #") + channel + ". ") + msg );
		//JOINFAILED channame {reason}
	}
	else if ( cmd == "CHANNELMESSAGE") )
	{
		channel = GetWordParam( params );
		m_se->OnChannelMessage( channel, params );
		//CHANNELMESSAGE channame {message}
	}
	else if ( cmd == "ACQUIREUSERID") )
	{
		SendCmd( "USERID"), Tostd::string( m_crc.GetCRC() ) );
	}
	else if ( cmd == "FORCELEAVECHANNEL") )
	{
		channel = GetWordParam( params );
		nick = GetWordParam( params );
		msg = GetSentenceParam( params );
		m_se->OnChannelPart( channel, GetMe().GetNick(), "Kicked by <") + nick + "> ") + msg );
		//FORCELEAVECHANNEL channame username [{reason}]
	}
	else if ( cmd == "DENIED") )
	{
		if ( m_online ) return;
		m_last_denied = msg = GetSentenceParam( params );
		m_se->OnServerMessage( msg );
		Disconnect();
		//Command: "DENIED" params: "Already logged in".
	}
	else if ( cmd == "HOSTPORT") )
	{
		unsigned int tmp_port = (unsigned int)GetIntParam( params );
		m_se->OnHostExternalUdpPort( tmp_port );
		//HOSTPORT port
	}
	else if ( cmd == "UDPSOURCEPORT") )
	{
		unsigned int tmp_port = (unsigned int)GetIntParam( params );
		m_se->OnMyExternalUdpSourcePort( tmp_port );
		if (m_do_finalize_join_battle)FinalizeJoinBattle();
		//UDPSOURCEPORT port
	}
	else if (cmd == "CLIENTIPPORT"))
	{
		// clientipport username ip port
		nick=GetWordParam( params );
		std::string ip=GetWordParam(params);
		unsigned int u_port=(unsigned int)GetIntParam( params );
		m_se->OnClientIPPort(nick, ip, u_port);
	}
	else if ( cmd == "SETSCRIPTTAGS") )
	{
		std::string command;
		while ( (command = GetSentenceParam( params )) != "") )
		{
			std::string key = command.BeforeFirst( '=' ).Lower();
			std::string value = command.AfterFirst( '=' );
			m_se->OnSetBattleInfo( m_battle_id, key, value );
		}
		m_se->OnBattleInfoUpdated( m_battle_id );
		// !! Command: "SETSCRIPTTAGS" params: "game/startpostype=0	game/maxunits=1000	game/limitdgun=0	game/startmetal=1000	game/gamemode=0	game/ghostedbuildings=-1	game/startenergy=1000	game/diminishingmms=0"
	}
	else if ( cmd == "SCRIPTSTART") )
	{
		m_se->OnScriptStart( m_battle_id );
		// !! Command: "SCRIPTSTART" params: ""
	}
	else if ( cmd == "SCRIPTEND") )
	{
		m_se->OnScriptEnd( m_battle_id );
		// !! Command: "SCRIPTEND" params: ""
	}
	else if ( cmd == "SCRIPT") )
	{
		m_se->OnScriptLine(  m_battle_id, params );
		// !! Command: "SCRIPT" params: "[game]"
	}
	else if ( cmd == "FORCEQUITBATTLE"))
	{
		m_relay_host_bot = "");
		m_se->OnKickedFromBattle();
	}
	else if ( cmd == "BROADCAST"))
	{
		m_se->OnServerBroadcast( params );
	}
	else if ( cmd == "SERVERMSGBOX"))
	{
		m_se->OnServerMessageBox( params );
	}
	else if ( cmd == "REDIRECT") )
	{
		if ( m_online ) return;
		std::string address = GetWordParam( params );
		unsigned int u_port = GetIntParam( params );
		if ( address.IsEmpty() ) return;
		if ( u_port  == 0 ) u_port  = DEFSETT_DEFAULT_SERVER_PORT;
		m_redirecting = true;
		m_se->OnRedirect( address, u_port , m_user, m_pass );
	}
	else if ( cmd == "MUTELISTBEGIN") )
	{
		m_current_chan_name_mutelist = GetWordParam( params );
		m_se->OnMutelistBegin( m_current_chan_name_mutelist );

	}
	else if ( cmd == "MUTELIST") )
	{
		std::string mutee = GetWordParam( params );
		std::string description = GetSentenceParam( params );
		m_se->OnMutelistItem( m_current_chan_name_mutelist, mutee, description );
	}
	else if ( cmd == "MUTELISTEND") )
	{
		m_se->OnMutelistEnd( m_current_chan_name_mutelist );
		m_current_chan_name_mutelist = "");
	}
	// OFFERFILE options {filename} {url} {description}
	else if ( cmd == "OFFERFILE") )
	{
		UTASOfferFileData parsingdata;
		parsingdata.data = GetIntParam( params );
		std::string FileName = GetSentenceParam( params );
		std::string url = GetSentenceParam( params );
		std::string description = GetSentenceParam( params );
		m_se->OnFileDownload( parsingdata.tasdata.autoopen, parsingdata.tasdata.closelobbyondownload, parsingdata.tasdata.disconnectonrefuse, FileName, url, description );
	}
	else
	{
		wxLogMessage( "??? Cmd: %s params: %s"), cmd.c_str(), params.c_str() );
		m_se->OnUnknownCommand( cmd, params );
	}
}
