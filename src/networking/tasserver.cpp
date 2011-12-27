#include "../utils/base64.h"
#include "../utils/md5.h"
#include "tasserver.h"
#include "iserver_private.h"
#include "tasserverdataformats.h"
#include "commandparsing.h"


TASServer::TASServer():
m_ser_ver(0),
m_server_lanmode(false),
{
	AddCommands();
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
	ailib += status.aishortname; // + "|" + status.aiversion;
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


void TASServer::OnAcceptAgreement()
{
	OnAcceptAgreement( m_agreement );
	m_agreement = "";
}

void TASServer::OnNewUser( const std::string& nick, const std::string& country, int cpu, int id )
{
	if(!id) id = GetNewUserId();
	User* user = GetUser( id );
	if ( !user ) user = AddUser( id );
	user->SetCountry( country );
	user->SetCpu( cpu );
	user->SetNick( nick );
	OnNewUser( user );
}

std::string TASServer::GetBattleChannelName( const Battle* battle )
{
	if (!battle) return "";
	return "B" + ToString(battle->GetID());
}

void TASServer::OnBattleOpened( int id, BattleType type, NatType nat, const std::string& nick,
								   const std::string& host, int port, int maxplayers,
								   bool haspass, int rank, const std::string& maphash, const std::string& map,
								   const std::string& title, const std::string& mod )
{
	Battle* battle = AddBattle( id );
	User* user = GetUser( nick );
	if (user) battle->OnUserAdded( user );

	battle->SetBattleType( type );
	battle->SetNatType( nat );
	battle->SetIsPassworded( haspass );
	battle->SetRankNeeded( rank );
	battle->SetDescription( title );

	OnBattleOpened( battle );
	OnBattleHostchanged( battle, user, host, port );
	if (user) OnUserIP( user, host );
	OnBattleMaxPlayersChanged(battle, maxplayers );
	OnBattleMapChanged( battle,UnitsyncMap(map, maphash) );
	OnBattleModChanged( battle, UnitsyncMod(mod, "") );

	std::string battlechanname = GetBattleChannelName(battle);
	Channel* channel = GetChannel( battlechanname );
	if (!channel)
	{
		channel = AddChannel( battlechanname );
		battle->SetChannel( channel );
	}

	if ( user->Status().in_game )
	{
		OnBattleStarted(battle);
	}
}

void TASServer::OnUserStatusChanged( const std::string& nick, int intstatus )
{
	User* user = GetUser( nick );
	if (!user) return;
	UTASClientStatus tasstatus;
	tasstatus.byte = intstatus;
	UserStatus status = ConvTasclientstatus( tasstatus.tasdata );
	OnUserStatusChanged( user, status );
	Battle* battle = user->GetBattle():
	if ( battle )
	{
		if ( battle->GetFounder() == user )
			if ( status.in_game != battle->GetInGame() )
			{
				battle->SetInGame( status.in_game );
				if ( status.in_game ) OnBattleStarted( battle );
				else OnBattleStopped( battle );
			}
	}
}



void TASServer::OnHostedBattle( int battleid )
{
	Battle* battle = GetBattle( battleid );
	if(!battle) return;
	OnSelfHostedBattle(battle);
	OnSelfJoinedBattle(battle);
}



int TASServer::GetNewUserId()
{
	// if server didn't send any account id to us, fill with an always increasing number
	m_account_id_count++;
	return m_account_id_count;
}



void TASServer::OnUserQuit( const std::string& nick )
{
	User* user = GetUser( nick );
	if ( !user ) return;
	OnUserQuit( user );
}


void TASServer::OnSelfJoinedBattle( int battleid, const std::string& hash )
{
	Battle* battle = GetBattle( battleid );
	if ( !battle ) return;
	m_battle = battle;
	battle.SetHostMod( battle.GetHostModName(), hash );

	UserBattleStatus& bs = m_me->BattleStatus();
	bs.spectator = false;

	OnUserJoinedBattle(battle,m_me);
}

void TASServer::OnStartHostedBattle()
{
	Battle* battle = m_battle;
	battle->SetInGame( true );
	OnBattleStarted( battle );
}

void TASServer::OnClientBattleStatus( const std::string& nick, int intstatus, int colourint )
{
	Battle* battle = m_battle;
	User* user = GetUser( nick );
	if ( !battle ) return;
	if ( !user ) return;
	UTASBattleStatus tasbstatus;
	UserBattleStatus bstatus;
	UTASColor color;
	tasbstatus.data = intstatus;
	bstatus = ConvTasbattlestatus( tasbstatus.tasdata );
	color.data = colourint;
	bstatus.colour = Colour( color.color.red, color.color.green, color.color.blue );
	if ( user->GetBattle() != battle ) return;
	user->BattleStatus().color_index = status.color_index;
	OnClientBattleStatus( battle, user, status );
}


void TASServer::OnUserJoinedBattle( int battleid, const std::string& nick, const std::string& userScriptPassword )
{
	User* user = GetUser(nick);
	Battle* battle = GetBattle( battleid );
	if ( !battle ) return;
	if ( !user ) return;

	battle->OnUserAdded( user );
	OnUserJoinedBattle( battle, user );
	if ( user == m_me ) m_battle = battle;
	OnUserScriptPassword( user, userScriptPassword );
	Channel* channel = battle->GetChannel();
	if (channel) OnUserJoinedChannel( channel, user );

	if ( user == battle.GetFounder() )
	{
		if ( user->Status().in_game )
		{
			OnBattleStarted(battle);
		}
	}


}


void TASServer::OnUserLeftBattle( int battleid, const std::string& nick )
{
	User* user = GetUser(nick);
	Battle* battle = GetBattle( battleid );
	if (!user) return;
	if(battle)
	{
		Channel* channel = battle->GetChannel();
		if (channel) OnUserLeftChannel( channel, user );
	}
	OnUserLeftBattle(battle, user);
	if ( user == m_me ) m_battle = 0;
}



void TASServer::OnBattleInfoUpdated( int battleid, int spectators, bool locked, const std::string& maphash, const std::string& map )
{
	Battle* battle = GetBattle( battleid );
	if ( !battle ) return;
	if (battle->GetNumSpectators() != spectators ) OnBattleSpectatorCountUpdated( battle, spectators );
	if (battle->IsLocked() != locked ) OnBattleSpectatorCountUpdated( battle, locked );
	if (battle->GetHostMapName() != mapname ) OnBattleMapChanged( battle, UnitsyncMap(mapname, maphash) );
}


void TASServer::OnSetBattleOption( const std::string& key, const std::string& value )
{
	Battle* battle = m_battle;
	if (!battle) return;
	battle->m_script_tags[param] = value;
	std::string key = param;
	if ( key.Left( 5 ) == "game/" )
	{
		key = key.AfterFirst( '/' );
		else if ( key.Left( 8 ) == "restrict" )
		{
			OnBattleDisableUnit( battleid, key.AfterFirst(_T('/')), atoi(value) );
		}
		else if ( key.Left( 4 ) ==  "team" ) && key.Find( "startpos" ) != wxNOT_FOUND )
		{
			int team = s2l( key.BeforeFirst(_T('/')).Mid( 4 ) );
			if ( key.Find( "startposx" ) != wxNOT_FOUND )
			{
				UserVec users = battle->GetUsers();
				for ( UserVec::iterator itor = users.begin(); users.end(); itor++ )
				{
					UserBattleStatus& status = itor->BattleStatus();
					if ( status.team == team )
					{
						status.pos.x = atoi( value );
						OnUserStartPositionUpdated(battle,user, status.pos.x, status.pos.y);
					}
				}
			 }
			 else if ( key.Find( "startposy" ) != wxNOT_FOUND )
			 {
				UserVec users = battle->GetUsers();
				for ( UserVec::iterator itor = users.begin(); users.end(); itor++ )
				{
					UserBattleStatus& status = itor->BattleStatus();
					if ( status.team == team )
					{
						status.pos.y = atoi( value );
						OnUserStartPositionUpdated(battle,user, status.pos.x, status.pos.y);
					}
				}
			}
		}
	}
	else iServer::OnSetBattleOption(battle, const std::string& param, const std::string& value );
}

void TASServer::OnSetBattleInfo( const std::string& infos )
{
	Battle* battle = m_battle;
	if (!battle) return;
	std::string command;
	while ( (command = GetSentenceParam( infos )) != "") )
	{
		std::string key = command.BeforeFirst( '=' ).Lower();
		std::string value = command.AfterFirst( '=' );
		OnSetBattleOption( key, value );
	}
}



void TASServer::OnBattleClosed( int battleid )
{
	Battle* battle = GetBattle( battleid );
	if (!battle) return;
	OnBattleClosed(battle);
}


void TASServer::OnBattleDisableUnits( const std::string& unitlist )
{
	Battle* battle = m_battle;
	if (!battle) return;
	StringVector unitlist = StringTokenize(unitnames," ");
	for (std::iterator itor = unitlist.begin(); itor != unitlist.end();itor++)
	{
		OnBattleDisableUnit( battle, *itor, 0 );
	}
}

void TASServer::OnBattleDisableUnit( const std::string& unitname, int count )
{
	Battle* battle = m_battle;
	if (!battle) return;
	OnBattleDisableUnit( battle, unitname, count );
}

void TASServer::OnBattleEnableUnits( const std::string& unitnames )
{
	Battle* battle = m_battle;
	if (!battle) return;
	StringVector unitlist = StringTokenize(unitnames," ")
	OnBattleEnableUnits( battle, unitlist );
}


void TASServer::OnBattleEnableAllUnits( int battleid )
{
	Battle* battle = GetBattle( battleid );
	if (!battle) return;
	OnBattleEnableAllUnits(battle);
}

void TASServer::OnJoinChannel( const std::string& channel )
{
	Channel* chan = AddChannel( "#" + channel );
	if(!channel) return;
	OnUserJoinedChannel( chan, m_me );

}

void TASServer::OnJoinChannelFailed( const std::string& channel, const std:string& reason )
{

	Channel* chan = GetChannel( "#" + channel );
	if(!channel) chan = AddChannel( "#" + channel );
	OnJoinChannelFailed( chan, reason );
}

void TASServer::OnChannelJoin( const std::string& channel, const std::string& who )
{
	Channel* channel = GetChannel( "#" + channel );
	User* user = GetUser( who );
	if(!channel) return;
	if(!user) return;
	OnUserJoinedChannel(channel, user);
}

void TASServer::OnChannelJoinUserList( const std::string& channel, const std::string& users )
{
	Channel* channel = GetChannel( "#" + channel );
	if(!channel) return;
	StringVector usernicks = stringtokenize(users," ");
	UserVector users;
	for ( StringVector::iterator itor = usernicks.begin();itor != usernicks.end;itor++)
		User* user = GetUser( *itor );
		if(!user) continue;
		users.push_back(user);
	}
	OnChannelJoinUserList(channel,users);
}


void TASSrver::OnHandle()
{
	SendCmd( "USERID", Tostring( m_crc.GetCRC() );
}

void TASServer::OnUserJoinedChannel( const std::string& channel, const std::string& who )
{
	Channel* channel = GetChannel( "#" + channel );
	User* user = GetUser( who );
	if(!channel) return;
	if(!user) return;
	OnUserJoinedChannel( channel, user );
}

void TASServer::OnChannelSaid( const std::string& channel, const std::string& who, const std::string& message )
{
	Channel* channel = GetChannel( "#" + channel );
	User* user = GetUser( who );
	if(!channel) return;
	if(!user) return;
	OnChannelSaid( channel, user, message );
}

void TASServer::OnChannelPart( const std::string& channel, const std::string& who, const std::string& message )
{
	Channel* channel = GetChannel( "#" + channel );
	User* user = GetUser( who );
	if(!channel) return;
	if(!user) return;
	OnChannelPart( channel, user, message );
}

void TASServer::OnChannelTopic( const std::string& channel, const std::string& who, int /*unused*/, const std::string& message )
{
	Channel* channel = GetChannel( "#" + channel );
	if(!channel) return;
	message = message.Replace( "\\n", "\n" );
	OnChannelTopic( channel, who, message );
}

void TASServer::OnChannelAction( const std::string& channel, const std::string& who, const std::string& action )
{
	Channel* channel = GetChannel( "#" + channel );
	User* user = GetUser( who );
	if(!channel) return;
	if(!user) return;
	OnChannelAction( channel, user, message );
}

Channel* TASServer::GetCreatePrivateChannel( const User* user )
{
	if (!user) return;
	std::string channame = "U" + ToString(user->GetID());
	Channel* channel = GetChannel( channame );
	if (!channel)
	{
		channel = AddChannel( channame );
		OnUserJoinedChannel( user, user );
		OnUserJoinedChannel( user, m_me );
	}
	return channel;
}

//! our own outgoing messages, user is destinatary
void TASServer::OnSayPrivateMessageEx( const std::string& user, const std::string& action )
{
	User* usr = GetUser( user );
	if(!usr) return;
	Channel* channel = GetCreatePrivateChannel(usr);
	OnChannelAction( channel, m_me, action );
}

//! incoming messages, user is source
void TASServer::OnSaidPrivateMessageEx( const std::string& user, const std::string& action )
{
	User* usr = GetUser( user );
	if(!usr) return;
	Channel* channel = GetCreatePrivateChannel(usr);
	OnChannelAction( channel, usr, action );
}

//! our own outgoing messages, user is destinatary
void TASServer::OnSayPrivateMessage( const std::string& user, const std::string& message )
{
	User* usr = GetUser( user );
	if(!usr) return;
	Channel* channel = GetCreatePrivateChannel(usr);
	OnChannelSaid( channel, m_me, message );
}

//! incoming messages, user is source
void TASServer::OnSaidPrivateMessage( const std::string& user, const std::string& message )
{
	User* usr = GetUser( user );
	if(!usr) return;
	Channel* channel = GetCreatePrivateChannel(usr);
	OnChannelSaid( channel, usr, message );
}

void TASServer::OnSaidBattle( const std::string& nick, const std::string& msg )
{
	User* usr = GetUser( user );
	if(!usr) return;
	Battle* battle = m_battle;
	if ( !battle ) return;
	Channel* channel = battle->GetChannel();
	if (!channel) return;
	OnChannelSaid( channel, user, message );
}

void TASServer::OnBattleAction( const std::string& nick, const std::string& msg )
{
	User* usr = GetUser( user );
	if(!usr) return;
	Battle* battle = m_battle;
	if ( !battle ) return;
	Channel* channel = battle->GetChannel();
	if (!channel) return;
	OnChannelSaid( channel, user, message );
}

void TASServer::OnBattleStartRectAdd( int allyno, int left, int top, int right, int bottom )
{
	Battle* battle = m_battle;
	if ( !battle ) return;
	OnBattleStartRectAdd(battle, allyno, left, top, right, bottom)
}


void TASServer::OnBattleStartRectRemove( int allyno )
{
	Battle* battle = m_battle;
	if ( !battle ) return;
	OnBattleStartRectRemove(battle, allyno);
}


void TASServer::OnScriptStart()
{
	Battle* battle = m_battle;
	if ( !battle ) return;
	battle->ClearScript();
}

void TASServer::OnScriptLine( const std::string& line )
{
	Battle* battle = m_battle;
	if ( !battle ) return;
	battle->AppendScriptLine( line );
}

void TASServer::OnScriptEnd()
{
	Battle* battle = m_battle;
	if ( !battle ) return;
	OnBattleScript( battle, battle->GetScript() );

}

void TASServer::OnMutelistBegin( const std::string& channel )
{
	m_mutelist.clear();
	m_current_chan_name_mutelist = channel;
}

void TASServer::OnMutelistItem( const std::string& mutee, const std::string& description )
{
	MuteListEntry entry;
	entry.who = mutee;
	entry.message = message;
	m_mutelist.push_back(entry);
}

void TASServer::OnMutelistEnd()
{
	Channel* chan = GetChannel("#" + m_current_chan_name_mutelist);
	m_current_chan_name_mutelist = "";
	if (!chan) return;
	OnMuteList(chan, m_mutelist);
}


void TASServer::OnChannelMessage( const std::string& channel, const std::string& msg )
{
	Channel* chan = GetChannel(channel);
	if (!chan) return;
	OnChannelMessage( chan, msg );
}




void TASServer::OnRing( const std::string& from )
{
	OnRing(GetUser(from));
}

void TASServer::OnKickedFromBattle()
{
	OnKickedFromBattle(m_battle);
	OnUserLeftBattle(m_battle,m_me);
}

void TASServer::OnKickedFromChannel( const std::string& channel, const std::string& fromWho, const std::string& msg)
{
	Channel* chan = GetChannel(channel);
	if(!chan) return;
	OnKickedFromChannel(chan, fromWho, message);
	OnUserLeftChannel(chan, m_me, message );
}

void TASServer::OnMyInternalUdpSourcePort( const unsigned int udpport )
{
	OnUserInternalUdpPort( m_me, udpport );
}


void TASServer::OnMyExternalUdpSourcePort( const unsigned int udpport )
{
	OnUserExternalUdpPort( m_me, udpport );
}


void TASServer::OnClientIPPort( const std::string &username, const std::string &ip, unsigned int udpport )
{
	User* user = GetUser( username );
	if (!user) return;

	OnUserIP( user, ip );
	OnUserExternalUdpPort( user, udpport )
}


void TASServer::OnHostExternalUdpPort( const int udpport )
{
	if (!m_battle) return;
	const User* host = m_battle->GetFounder();
	OnUserExternalUdpPort(m_battle->GetFounder(), udpport);
	OnBattleHostchanged( m_battle, host, battle->GetIP(),udpport );
}

void TASServer::OnChannelListEntry( const std::string& channel, const int& numusers, const std::string& topic )
{
	Channel* chan = GetChannel( "#" + channel );
	if (!chan)
	{
		chan = AddChannel( "#" + channel );
	}
	chan->SetNumUsers(numusers);
	chan->SetTopic(topic);
}


void TASServer::OnAgreenmentLine( const std::string& line )
{
	m_agreement += line + "\n";
}

void TASServer::OnRequestBattleStatus()
{
	OnRequestBattleStatus(m_battle);
}



void TASServer::OnBattleAddBot( int battleid, const std::string& nick, const std::string& owner, int intstatus, int intcolour, const std::string& aidll)
{
	Battle* battle = GetBattle(battleid);
	if (!battle) return;
	UTASBattleStatus tasbstatus;
	UserBattleStatus status;
	UTASColor color;
	tasbstatus.data =intstatus;
	status = ConvTasbattlestatus( tasbstatus.tasdata );
	color.data = intcolour;
	status.colour = Colour( color.color.red, color.color.green, color.color.blue );
	status.aishortname = aidll;
	status.isbot = true;
	User* owneruser = GetUser( owner );
	if ( owneruser ) bstatus.owner = owneruser;
	User* user = OnUserAdded( nick, "", -1, GetNextAvailableID() );
	OnUserJoinedBattle( battle, user );
	OnUserBattleStatusUpdated( battle, user, status );
}

void TASServer::OnBattleUpdateBot( int battleid, const std::string& nick, int intstatus, int intcolour )
{
	Battle* battle = GetBattle(battleid);
	if (!battle) return;
	UTASBattleStatus tasbstatus;
	UserBattleStatus status;
	UTASColor color;
	tasbstatus.data =intstatus;
	status = ConvTasbattlestatus( tasbstatus.tasdata );
	color.data = intcolour;
	status.colour = Colour( color.color.red, color.color.green, color.color.blue );
	User* user = battle->GetUser( nick );
	OnUserBattleStatusUpdated( battle, user, status );
}


void TASServer::OnBattleRemoveBot( int battleid, const std::string& nick )
{
	Battle* battle = GetBattle(battleid);
	if (!battle) return;
	User* user = battle->GetUser( nick );
	if (!user ) return;
	OnUserLeftBattle( battle, nick );
	if (user->BattleStatus().isbot) OnUserQuit( user );
}

void iServer::OnFileDownload( int intdata, const std::string& FileName, const std::string& url, const std::string& description )
{
	UTASOfferFileData parsingdata;
	parsingdata.data = intdata;
	OnFileDownload(  parsingdata.tasdata.autoopen, parsingdata.tasdata.closelobbyondownload, parsingdata.tasdata.disconnectonrefuse, FileName, url, description );
}

void TASServer::ExecuteCommand( const std::string& cmd, const std::string& inparams, int replyid )
{
	if ( cmd == "PONG") ) HandlePong( replyid );
	else s.cmd_map_[cmd]->process(inparams);
}
