#include "tasserver.h"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <unitsync++/optionswrapper.h>

#include <lslutils/base64.h>
#include <lslutils/md5.h>
#include <lslutils/conversion.h>
#include <lslutils/debug.h>
#include <lsl/battle/battle.h>

#include "socket.h"
#include "commands.h"
#include "tasserverdataformats.h"

#define ASSERT_EXCEPTION(cond,msg) do { if (!(cond)) { LSL_THROW( server, msg ); } } while (0)

namespace LSL {

TASServer::TASServer(/*int TASServerMode*/)
	: m_cmd_dict( new CommandDictionary(this) )
    , m_account_id_count(0)
{
	m_sock->sig_dataReceived.connect( boost::bind( &TASServer::ExecuteCommand, this, _1, _2 ) );
}

void TASServer::ExecuteCommand(const std::string& cmd, std::string& inparams, int replyid )
{
    if ( cmd == "PONG")
        HandlePong( replyid );
    else
		m_cmd_dict->Process(cmd,inparams);
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
	SendCmd( "CHANNELTOPIC", boost::replace_all_copy( topic, "\n", "\\n" ) );
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
	assert( false );
//	return GetPasswordHash(params);
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

int TASServer::Register( const std::string& addr, const int port, const std::string& nick,
                         const std::string& password, std::string& reason )
{
	assert( false );
#if 0
	FakeNetClass temp;
	Socket tempsocket( temp, true, true );
	tempsocket.Connect( addr, port );
	if ( tempsocket.State() != Enum::SS_Open )
		return false;

	std::string data = tempsocket.Receive().BeforeLast("\n");
	if ( data.find( "\r" ) != std::string::npos )
		data = Util::BeforeLast( data, "\r" );
	if ( GetWordParam( data ) != "TASServer" )
		return false;

	tempsocket.Send( "REGISTER " + nick + " " + GetPasswordHash( password ) + "\n" );

	data = tempsocket.Receive().BeforeLast('\n');
	tempsocket.Disconnect();
    if ( data.find( "\r" ) != std::string::npos ) data = data.BeforeLast('\r');
	if ( data.length() > 0 )
	{
		return 1;
	}
	std::string cmd = GetWordParam( data );
	if ( cmd == "REGISTRATIONACCEPTED")
	{
		return 0;
	}
	else if ( cmd == "REGISTRATIONDENIED" )
	{
        reason = data;
		return 2;
	}
#endif
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
	if ( localaddr.length() < 1 ) localaddr = "*";
    SendCmd ( "LOGIN", user + " " + pass + " " + Util::GetHostCPUSpeed() + " "
			  + localaddr + " liblobby " + Util::GetLibLobbyVersion() + protocol  + "\ta sp");
}

void TASServer::RequestChannels()
{
	SendCmd( "CHANNELS" );
}

void TASServer::AcceptAgreement()
{
	SendCmd( "CONFIRMAGREEMENT" );
}

void TASServer::ExecuteCommand( const std::string& cmd, std::string& params )
{
	int replyid = 0;
	if ( params[0] == '#' )
	{
		std::string id = Util::AfterFirst( Util::BeforeFirst( params, " " ),  "#" );
		params = Util::AfterFirst( params, " " );
		replyid = Util::FromString<int>( id );
	}
	ExecuteCommand( cmd, params, replyid );
}

void TASServer::SendCmd( const std::string& command, const std::string& param )
{
    std::string msg;
	GetLastID()++;
	msg = msg + "#" + Util::ToString( GetLastID() ) + " ";
	if ( !param.length() ) msg = msg + command + "\n";
	else msg = msg + command + " " + param + "\n";
	bool send_success = m_sock->SendData( msg );
	sig_SentMessage(send_success, msg, GetLastID());
}

void TASServer::SendPing()
{
	SendCmd( "PING" );
}

void TASServer::JoinChannel( const std::string& channel, const std::string& key )
{
	//JOIN channame [key]
	SendCmd ( "JOIN", channel + " " + key );
}

void TASServer::PartChannel( const std::string& channel )
{
	SendCmd( "LEAVE", channel );
}

void TASServer::DoActionChannel( const std::string& channel, const std::string& msg )
{
	SendCmd( "SAYEX", channel + " " + msg );
}

void TASServer::SayChannel( const std::string& channel, const std::string& msg )
{
	SendCmd( "SAY", channel + " " + msg );
}

void TASServer::SayPrivate( const std::string& nick, const std::string& msg )
{
	SendCmd( "SAYPRIVATE", nick + " " + msg );
}

void TASServer::DoActionPrivate( const std::string& nick, const std::string& msg )
{
	SendCmd( "SAYPRIVATEEX", nick + " " + msg );
}

void TASServer::SayBattle( int /*unused*/, const std::string& msg )
{
	SendCmd( "SAYBATTLE", msg );
}

void TASServer::DoActionBattle( int /*unused*/, const std::string& msg )
{
	SendCmd( "SAYBATTLEEX", msg );
}

void TASServer::Ring( const ConstUserPtr user )
{
	if ( m_current_battle && m_current_battle->IsProxy() )
        RelayCmd( "RING", user->Nick() );
	else
        SendCmd( "RING", user->Nick() );
}

void TASServer::ModeratorSetChannelTopic( const std::string& channel, const std::string& topic )
{
	SendCmd( "CHANNELTOPIC", channel + " " + boost::replace_all_copy( topic, "\n", "\\n" ) );
}

void TASServer::ModeratorSetChannelKey( const std::string& channel, const std::string& key)
{
	SendCmd( "SETCHANNELKEY", channel + " " + key );
}

void TASServer::ModeratorMute( const std::string& channel, const std::string& nick, int duration, bool byip )
{
//	SendCmd( "MUTE", channel + " " + nick + " " + boost::format( "%d"), duration) + (byip?" ip":"")  );
	SendCmd( "MUTE", boost::format( "%s %s %s %d %s" ) % channel % nick % duration % (byip?" ip":"")  );
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

void TASServer::_HostBattle( Battle::BattleOptions bo )
{
    boost::format cmd( "0 %d %s %d %d %s %d %s %s\t%s\t");
    cmd % bo.nattype
        % (bo.password.empty() ? "*" : bo.password)
        % bo.port % bo.maxplayers
        % Util::MakeHashSigned( bo.modhash )
        % bo.rankneeded
        % Util::MakeHashSigned( bo.maphash )
        % bo.mapname
        % bo.description
        % bo.modname;

	m_delayed_open_command = "";
	if ( !bo.userelayhost )
	{
       SendCmd( "OPENBATTLE", cmd );
	}
	else
	{
		m_delayed_open_command = cmd.str();
	}

	// OPENBATTLE type natType password port maphash {map} {title} {modname}
}

void TASServer::_JoinBattle( const IBattlePtr battle, const std::string& password, const std::string& scriptpassword )
{
    SendCmd( "JOINBATTLE", Util::ToString(battle->Id()) + " " + password + " " + scriptpassword );
}

void TASServer::LeaveBattle( const int& /*unused*/ )
{
	//LEAVEBATTLE
	SendCmd( "LEAVEBATTLE" );
}

void TASServer::SendHostInfo( Enum::HostInfo update )
{
	if (!m_current_battle) return;
	if (!m_current_battle->IsFounderMe()) return;

	if ( ( update & ( Enum::HI_Map | Enum::HI_Locked | Enum::HI_Spectators ) ) > 0 )
	{
		// UPDATEBATTLEINFO SpectatorCount locked maphash {mapname}
		std::string cmd = (boost::format( "%d %d ") % m_current_battle->GetSpectators() % m_current_battle->IsLocked() ).str();
		cmd += Util::MakeHashSigned( m_current_battle->LoadMap().hash ) + " ";
		cmd += m_current_battle->LoadMap().name;
		RelayCmd( "UPDATEBATTLEINFO", cmd );
	}
	int relayhostmessagesize = 0;
	if (m_relay_host_bot)
	{
		relayhostmessagesize = m_relay_host_bot->Nick().length() + 10 + 1 + 1 + 14 + 1; // bot name + SAYPRIVATE + space + "!" + SETSCRIPTTAGS + \n
	}
	if ( ( update & Enum::HI_Send_All_opts ) > 0 )
	{
		std::string cmd;
		OptionsWrapper::stringTripleVec optlistMap = m_current_battle->CustomBattleOptions()->getOptions( OptionsWrapper::MapOption );
		for (OptionsWrapper::stringTripleVec::const_iterator it = optlistMap.begin(); it != optlistMap.end(); ++it)
		{
			std::string newcmd = "game/mapoptions/" + it->first + "=" + it->second.second + "\t";
			if ( relayhostmessagesize + cmd.length() + newcmd.length() > m_message_size_limit )
			{
				RelayCmd( "SETSCRIPTTAGS", cmd );
				cmd = "";
			}
			cmd += newcmd;
		}
		OptionsWrapper::stringTripleVec optlistMod = m_current_battle->CustomBattleOptions()->getOptions( OptionsWrapper::ModOption );
		for (OptionsWrapper::stringTripleVec::const_iterator it = optlistMod.begin(); it != optlistMod.end(); ++it)
		{
			std::string newcmd = "game/modoptions/" + it->first + "=" + it->second.second + "\t";
			if (relayhostmessagesize + cmd.length() + newcmd.length() > m_message_size_limit  )
			{
				RelayCmd( "SETSCRIPTTAGS", cmd );
				cmd = "";
			}
			cmd += newcmd;
		}
		OptionsWrapper::stringTripleVec optlistEng = m_current_battle->CustomBattleOptions()->getOptions( OptionsWrapper::EngineOption );
		for (OptionsWrapper::stringTripleVec::const_iterator it = optlistEng.begin(); it != optlistEng.end(); ++it)
		{
			std::string newcmd = "game/" + it->first + "=" + it->second.second + "\t";
			if ( relayhostmessagesize + cmd.length() + newcmd.length() > m_message_size_limit  )
			{
				RelayCmd( "SETSCRIPTTAGS", cmd );
				cmd = "";
			}
			cmd += newcmd;
		}
		RelayCmd( "SETSCRIPTTAGS", cmd );
	}

	if ( (update & Enum::HI_StartRects) > 0 )   // Startrects should be updated.
	{
		unsigned int numrects = m_current_battle->GetLastRectIdx();
		for ( unsigned int i = 0; i <= numrects; i++ )   // Loop through all, and remove updated or deleted.
		{
			Battle::BattleStartRect sr = m_current_battle->GetStartRect( i );
			if ( !sr.exist ) continue;
			if ( sr.todelete )
			{
				RelayCmd( "REMOVESTARTRECT", Util::ToString(i) );
				m_current_battle->StartRectRemoved( i );
			}
			else if ( sr.toadd )
			{
				RelayCmd( "ADDSTARTRECT", boost::format( "%d %d %d %d %d") % sr.ally % sr.left % sr.top % sr.right % sr.bottom );
				m_current_battle->StartRectAdded( i );
			}
			else if ( sr.toresize )
			{
				RelayCmd( "REMOVESTARTRECT", Util::ToString(i) );
				RelayCmd( "ADDSTARTRECT", boost::format( "%d %d %d %d %d") % sr.ally % sr.left % sr.top % sr.right % sr.bottom );
				m_current_battle->StartRectResized( i );
			}
		}
	}
	if ( (update & Enum::HI_Restrictions) > 0 )
	{
		std::map<std::string, int> units = m_current_battle->RestrictedUnits();
		RelayCmd( "ENABLEALLUNITS" );
		if ( units.size() > 0 )
		{
			std::stringstream msg;
			std::stringstream scriptmsg;
			for ( std::map<std::string, int>::const_iterator itor = units.begin(); itor != units.end(); itor++ )
			{
				 msg << itor->first + " ";
				 scriptmsg << "game/restrict/" + itor->first + "=" + Util::ToString(itor->second) + '\t'; // this is a serious protocol abuse, but on the other hand, the protocol fucking suck and it's unmaintained so it will do for now
			}
			RelayCmd( "DISABLEUNITS", msg.str() );
			RelayCmd( "SETSCRIPTTAGS", scriptmsg.str() );
		}
	}
}

void TASServer::SendHostInfo( int type, const std::string& key )
{
	if (!m_current_battle) return;
	if (!m_current_battle->IsFounderMe()) return;

	std::string cmd;

	if ( type == OptionsWrapper::MapOption )
	{
		cmd = "game/mapoptions/" + key + "=" + m_current_battle->CustomBattleOptions()->getSingleValue( key, OptionsWrapper::MapOption );
	}
	else if ( type == OptionsWrapper::ModOption )
	{
		cmd = "game/modoptions/" + key + "=" + m_current_battle->CustomBattleOptions()->getSingleValue( key, OptionsWrapper::ModOption );
	}
	else if ( type == OptionsWrapper::EngineOption )
	{
		cmd = "game/" + key + "=" + m_current_battle->CustomBattleOptions()->getSingleValue( key, OptionsWrapper::EngineOption );
	}
	RelayCmd( "SETSCRIPTTAGS", cmd );
}

void TASServer::SendUserPosition( const UserPtr user )
{
	if (!m_current_battle) return;
	if (!m_current_battle->IsFounderMe()) return;
	if (!user) return;

	UserBattleStatus status = user->BattleStatus();
	std::string msgx = "game/Team" + Util::ToString( status.team ) + "/StartPosX=" + Util::ToString( status.pos.x );
	std::string msgy = "game/Team" + Util::ToString( status.team ) + "/StartPosY=" + Util::ToString( status.pos.y );
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

void TASServer::SendMyBattleStatus( UserBattleStatus& bs )
{
	UTASBattleStatus tasbs;
	tasbs.tasdata = ConvTasbattlestatus( bs );
	UTASColor tascl;
	tascl.color.red = bs.color.Red();
	tascl.color.green = bs.color.Green();
	tascl.color.blue = bs.color.Blue();
	tascl.color.zero = 0;
	//MYBATTLESTATUS battlestatus myteamcolor
    SendCmd( "MYBATTLESTATUS", boost::format( "%d %d") % tasbs.data % tascl.data );
}

void TASServer::SendMyUserStatus()
{
	UserStatus& us = GetMe()->Status();

	UTASClientStatus taus;
	taus.tasdata.in_game = us.in_game;
	taus.tasdata.away = us.away;
	taus.tasdata.rank = us.rank;
	taus.tasdata.moderator = us.moderator;
	taus.tasdata.bot = us.bot;
	SendCmd( "MYSTATUS", Util::ToString( taus.byte ) );
}

void TASServer::StartHostedBattle()
{

}

void TASServer::ForceSide( const BattlePtr battle, const UserPtr user, int side )
{
	if (!battle) return;
	if (!user) return;
	UserBattleStatus status = user->BattleStatus();
	if (!m_current_battle->IsFounderMe()) return;
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

void TASServer::ForceTeam( const BattlePtr battle, const UserPtr user, int team )
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
	if (!m_current_battle->IsFounderMe()) return;

	//FORCETEAMNO username teamno
	RelayCmd( "FORCETEAMNO", user->Nick() + " " + Util::ToString(team) );
}

void TASServer::ForceAlly( const BattlePtr battle, const UserPtr user, int ally )
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
	if (!m_current_battle->IsFounderMe()) return;

	//FORCEALLYNO username teamno
	else RelayCmd( "FORCEALLYNO", user->Nick() + " " + Util::ToString(ally) );
}

void TASServer::ForceColor(const BattlePtr battle, const UserPtr user, int r , int g, int b)
{
	lslColor rgb(r,g,b);
	if (!battle) return;
	if (!user) return;
	UserBattleStatus status = user->BattleStatus();

	if ( status.IsBot() )
	{
		status.color = rgb;
		UpdateBot( battle, user, status );
		return;
	}
	if ( user == m_me )
	{
		status.color = rgb;
		SendMyBattleStatus( status );
		return;
	}
	if (!m_current_battle->IsFounderMe()) return;

	UTASColor tascl;
	tascl.color.red = rgb.Red();
	tascl.color.green = rgb.Green();
	tascl.color.blue = rgb.Blue();
	tascl.color.zero = 0;
	//FORCETEAMCOLOR username color
	RelayCmd( "FORCETEAMCOLOR", user->Nick() + " " + Util::ToString( tascl.data ) );
}

void TASServer::ForceSpectator( const BattlePtr battle, const UserPtr user, bool spectator )
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
	if (!m_current_battle->IsFounderMe()) return;

	//FORCESPECTATORMODE username
	RelayCmd( "FORCESPECTATORMODE", user->Nick() );
}

void TASServer::BattleKickPlayer( const BattlePtr battle, const UserPtr user )
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
        LeaveBattle( battle->Id() );
		return;
	}
	if (!m_current_battle->IsFounderMe()) return;

	//KICKFROMBATTLE username
	RelayCmd( "KICKFROMBATTLE", user->Nick() );
}

void TASServer::SetHandicap( const BattlePtr battle, const UserPtr user, int handicap)
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

	if (!m_current_battle->IsFounderMe()) return;

	//HANDICAP username value
	RelayCmd( "HANDICAP", user->Nick() + " " + Util::ToString(handicap) );
}

void TASServer::AddBot( const BattlePtr battle, const std::string& nick, UserBattleStatus& status )
{
	if (!battle) return;

	UTASBattleStatus tasbs;
	tasbs.tasdata = ConvTasbattlestatus( status );
	UTASColor tascl;
	tascl.color.red = status.color.Red();
	tascl.color.green = status.color.Green();
	tascl.color.blue = status.color.Blue();
	tascl.color.zero = 0;
	//ADDBOT name battlestatus teamcolor {AIDLL}
	std::string msg;
	std::string ailib;
	ailib += status.aishortname; // + "|" + status.aiversion;
	SendCmd( "ADDBOT", nick + Util::ToString(tasbs.data) + " " + Util::ToString( tascl.data ) + " " + ailib );
}

void TASServer::RemoveBot( const BattlePtr battle, const UserPtr user )
{
	if (!battle) return;
	if (!user) return;
	UserBattleStatus status = user->BattleStatus();
	if (!status.IsBot()) return;

	//REMOVEBOT name
	RelayCmd( "REMOVEBOT", user->Nick() );
}

void TASServer::UpdateBot( const BattlePtr battle, const UserPtr bot, UserBattleStatus& status )
{
	if (!battle) return;
    if (!bot) return;
    if (!status.IsBot()) return;

	UTASBattleStatus tasbs;
	tasbs.tasdata = ConvTasbattlestatus( status );
	UTASColor tascl;
	tascl.color.red = status.color.Red();
	tascl.color.green = status.color.Green();
	tascl.color.blue = status.color.Blue();
	tascl.color.zero = 0;
    //UPDATEBOT name battlestatus teamcolor
    boost::format params( "%s %d %d");
    params % bot->Nick() % tasbs.data % tascl.data;
    if( !battle->IsProxy() )
        SendCmd( "UPDATEBOT", params );
    else
        RelayCmd( "UPDATEBOT", params );
}

void TASServer::SendScriptToClients( const std::string& script )
{
	RelayCmd( "SCRIPTSTART" );
    const StringVector lines = Util::StringTokenize(script,"\n");
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
    iServer::OnAcceptAgreement( m_agreement );
	m_agreement = "";
}

void TASServer::OnNewUser( const std::string& nick, const std::string& country, int cpu, int id )
{
    if ( !id ) id = GetNewUserId();
    const std::string str_id = Util::ToString( id );
    UserPtr user = m_users.Get( str_id );
    if ( !user ) user = AddUser( id );
	user->SetCountry( country );
	user->SetCpu( cpu );
	user->SetNick( nick );
    iServer::OnNewUser( user );
}

std::string TASServer::GetBattleChannelName( const BattlePtr battle )
{
	if (!battle)
		return "";
    return "B" + Util::ToString(battle->Id());
}

void TASServer::OnBattleOpened( int id, Enum::BattleType type, Enum::NatType nat, const std::string& nick,
								   const std::string& host, int port, int maxplayers,
								   bool haspass, int rank, const std::string& maphash, const std::string& map,
								   const std::string& title, const std::string& mod )
{
	BattlePtr battle = AddBattle( id );
    const UserPtr user = m_users.FindByNick( nick );
    battle->OnUserAdded( user );
	battle->SetBattleType( type );
	battle->SetNatType( nat );
	battle->SetIsPassworded( haspass );
	battle->SetRankNeeded( rank );
	battle->SetDescription( title );

    iServer::OnBattleOpened( battle );
    OnBattleHostChanged( battle, user, host, port );
	if (user) OnUserIP( user, host );
	OnBattleMaxPlayersChanged(battle, maxplayers );
	OnBattleMapChanged( battle,UnitsyncMap(map, maphash) );
	OnBattleModChanged( battle, UnitsyncMod(mod, "") );

    std::string battlechanname = m_battles.GetChannelName(battle);
    ChannelPtr channel = m_channels.Get( battlechanname );
	if (!channel)
	{
        m_channels.Add( channel );
		battle->SetChannel( channel );
	}

	if ( user->Status().in_game )
	{
        iServer::OnBattleStarted(battle);
	}
}

void TASServer::OnUserStatusChanged( const std::string& nick, int intstatus )
{
    const ConstUserPtr user = m_users.FindByNick( nick );
	if (!user) return;
	UTASClientStatus tasstatus;
	tasstatus.byte = intstatus;
	UserStatus status = ConvTasclientstatus( tasstatus.tasdata );
	sig_UserStatusChanged( user, status );
    BattlePtr battle = user->GetBattle();
	if ( battle )
	{
		if ( battle->GetFounder() == user )
            if ( status.in_game != battle->InGame() )
			{
				battle->SetInGame( status.in_game );
                if ( status.in_game )
                    OnBattleStarted( battle );
                else
                    OnBattleStopped( battle );
			}
	}
}

void TASServer::OnHostedBattle( int battleid )
{
    const BattlePtr battle = m_battles.Get( battleid );
	if(!battle) return;
	OnSelfHostedBattle(battle);
    iServer::OnSelfJoinedBattle(battle);
}

int TASServer::GetNewUserId()
{
	// if server didn't send any account id to us, fill with an always increasing number
	m_account_id_count++;
	return m_account_id_count;
}

void TASServer::OnUserQuit(const std::string &nick )
{
    UserPtr user = m_users.FindByNick( nick );
	if ( !user ) return;
    iServer::OnUserQuit( user );
}

void TASServer::OnSelfJoinedBattle( int battleid, const std::string& hash )
{
    BattlePtr battle = m_battles.Get( battleid );
	if ( !battle ) return;
    m_current_battle = battle;
    battle->SetHostMod( battle->GetHostModName(), hash );

	UserBattleStatus& bs = m_me->BattleStatus();
	bs.spectator = false;

    iServer::OnUserJoinedBattle(battle,m_me);
}

void TASServer::OnStartHostedBattle()
{
    IBattlePtr battle = m_current_battle;
	battle->SetInGame( true );
	OnBattleStarted( battle );
}

void TASServer::OnClientBattleStatus( const std::string& nick, int intstatus, int colorint )
{
    IBattlePtr battle = m_current_battle;
    UserPtr user = m_users.FindByNick( nick );
	if ( !battle ) return;
	if ( !user ) return;
	UTASBattleStatus tasbstatus;
	UserBattleStatus bstatus;
	UTASColor color;
	tasbstatus.data = intstatus;
	bstatus = ConvTasbattlestatus( tasbstatus.tasdata );
	color.data = colorint;
    bstatus.color = lslColor( color.color.red, color.color.green, color.color.blue );
    if ( user->GetBattle() != battle ) return;
    user->BattleStatus().color_index = bstatus.color_index;
    iServer::OnClientBattleStatus( battle, user, bstatus );
}

void TASServer::OnUserJoinedBattle( int battleid, const std::string& nick, const std::string& userScriptPassword )
{
    BattlePtr battle = m_battles.Get( battleid );
	if ( !battle ) return;
    UserPtr user = m_users.FindByNick( nick );
	if ( !user ) return;
    battle->OnUserAdded( user );
    iServer::OnUserJoinedBattle( battle, user );
    if ( user == m_me ) m_current_battle = battle;
	OnUserScriptPassword( user, userScriptPassword );
    const ChannelPtr channel = battle->GetChannel();
    if (channel)
        iServer::OnUserJoinedChannel( channel, user );

    if ( user == battle->GetFounder() )
	{
		if ( user->Status().in_game )
		{
			OnBattleStarted(battle);
		}
	}
}

void TASServer::OnUserLeftBattle( int battleid, const std::string& nick )
{
    UserPtr user = m_users.FindByNick(nick);
    BattlePtr battle = m_battles.Get( battleid );
	if (!user) return;
	if(battle)
	{
        const ChannelPtr channel = battle->GetChannel();
        if (channel)
            iServer::OnUserLeftChannel( channel, user );
	}
    iServer::OnUserLeftBattle(battle, user);
    if ( user == m_me ) m_current_battle = IBattlePtr();
}

void TASServer::OnBattleInfoUpdated( int battleid, int spectators, bool locked, const std::string& maphash, const std::string& mapname )
{
    BattlePtr battle = m_battles.Get( battleid );
	if ( !battle ) return;
    if (battle->GetSpectators() != spectators ) OnBattleSpectatorCountUpdated( battle, spectators );
	if (battle->IsLocked() != locked ) OnBattleSpectatorCountUpdated( battle, locked );
	if (battle->GetHostMapName() != mapname ) OnBattleMapChanged( battle, UnitsyncMap(mapname, maphash) );
}

void TASServer::OnSetBattleOption( std::string key, const std::string& value )
{
    IBattlePtr battle = m_current_battle;
	if (!battle) return;
    battle->m_script_tags[key] = value;
    if ( key.substr( 0,5 ) == "game/" )
	{
        key = Util::AfterFirst( key, "/" );
        //TODO the original had modoptions here???
        if ( key.substr( 0,8 ) == "restrict" )
		{
            iServer::OnBattleDisableUnit( battle, Util::AfterFirst(key,"/"), Util::FromString<int>(value) );
		}
        else if ( ( key.substr( 0,4 ) ==  "team" ) && key.find( "startpos" ) != std::string::npos )
		{
            int team = Util::FromString<int>( Util::BeforeFirst(key,"/").substr( 4, std::string::npos ) );
            if ( key.find( "startposx" ) != std::string::npos )
			{
                BOOST_FOREACH( const UserPtr player, battle->Users() )
				{
                    UserBattleStatus& status = player->BattleStatus();
					if ( status.team == team )
					{
                        status.pos.x = Util::FromString<int>( value );
                        OnUserStartPositionUpdated( battle, player, status.pos );
					}
				}
			 }
             else if ( key.find( "startposy" ) != std::string::npos )
			 {
                BOOST_FOREACH( const UserPtr player, battle->Users() )
				{
                    UserBattleStatus& status = player->BattleStatus();
					if ( status.team == team )
					{
                        status.pos.y = Util::FromString<int>( value );
                        OnUserStartPositionUpdated( battle, player, status.pos );
					}
				}
			}
        }
	}
	else
        iServer::OnSetBattleOption(battle, key, value );
}

void TASServer::OnSetBattleInfo( std::string infos )
{
    IBattlePtr battle = m_current_battle;
	if (!battle) return;
    BOOST_FOREACH( const std::string command,
                   Util::StringTokenize( infos, "\t", boost::algorithm::token_compress_on ) )
	{
        const std::string key = boost::algorithm::to_lower_copy( Util::BeforeFirst( command,"=" ) );
        const std::string value = Util::AfterFirst( command,"=" );
		OnSetBattleOption( key, value );
	}
}

void TASServer::OnBattleClosed( int battleid )
{
    BattlePtr battle = m_battles.Get( battleid );
	if (!battle) return;
    iServer::OnBattleClosed(battle);
}

void TASServer::OnBattleDisableUnits( const std::string& unitlist )
{
    IBattlePtr battle = m_current_battle;
	if (!battle) return;
    const StringVector units = Util::StringTokenize( unitlist, " " );
    BOOST_FOREACH( const std::string unit, units )
	{
        iServer::OnBattleDisableUnit( battle, unit, 0 );
	}
}

void TASServer::OnBattleDisableUnit( const std::string& unitname, int count )
{
    IBattlePtr battle = m_current_battle;
	if (!battle) return;
    iServer::OnBattleDisableUnit( battle, unitname, count );
}

void TASServer::OnBattleEnableUnits( const std::string& unitnames )
{
    IBattlePtr battle = m_current_battle;
	if (!battle) return;
    const StringVector unitlist = Util::StringTokenize(unitnames," ");
    iServer::OnBattleEnableUnits( battle, unitlist );
}

void TASServer::OnBattleEnableAllUnits()
{
    iServer::OnBattleEnableAllUnits(m_current_battle);
}

void TASServer::OnJoinChannel(const std::string& channel , const std::string &rest)
{
    ChannelPtr chan = m_channels.Get( "#" + channel );
    if(!chan) return;
    iServer::OnUserJoinedChannel( chan, m_me );
}

void TASServer::OnJoinChannelFailed( const std::string& name, const std::string& reason )
{
    ChannelPtr chan = m_channels.Get( "#" + name );
    if(!chan) chan = m_channels.Add( new Channel("#" + name) );
    iServer::OnJoinChannelFailed( chan, reason );
}

void TASServer::OnChannelJoin( const std::string& name, const std::string& who )
{
    ChannelPtr channel = m_channels.Get( "#" + name );
    UserPtr user = m_users.FindByNick( who );
    if(!channel) return;
	if(!user) return;
    iServer::OnUserJoinedChannel(channel, user);
}

void TASServer::OnChannelJoinUserList( const std::string& channel_name, const std::string& usernames )
{
    ChannelPtr channel = m_channels.Get( "#" + channel_name );
	if(!channel) return;
    UserVector users;
    BOOST_FOREACH( const std::string nick, Util::StringTokenize(usernames," ") )
    {
        UserPtr user = m_users.FindByNick( nick );
		if(!user) continue;
		users.push_back(user);
	}
    iServer::OnChannelJoinUserList(channel,users);
}

void TASServer::OnJoinedBattle(const int battleid, const std::string msg)
{
	assert( false );
}

void TASServer::OnGetHandle()
{
    SendCmd( "USERID", Util::ToString( m_crc.GetCRC() ) );
}

void TASServer::OnLogin(const std::string &msg)
{
	assert( false );
}

void TASServer::OnUserJoinedChannel( const std::string& channel_name, const std::string& who )
{
    ChannelPtr channel = m_channels.Get( "#" + channel_name );
    UserPtr user = m_users.FindByNick( who );
	if(!channel) return;
	if(!user) return;
    iServer::OnUserJoinedChannel( channel, user );
}

void TASServer::OnChannelSaid( const std::string& channel_name, const std::string& who, const std::string& message )
{
    ChannelPtr channel = m_channels.Get( "#" + channel_name );
    UserPtr user = m_users.FindByNick( who );
	if(!channel) return;
	if(!user) return;
    iServer::OnChannelSaid( channel, user, message );
}

void TASServer::OnChannelPart( const std::string& channel_name, const std::string& who, const std::string& message )
{
    ChannelPtr channel = m_channels.Get( "#" + channel_name );
    UserPtr user = m_users.FindByNick( who );
	if(!channel) return;
	if(!user) return;
    iServer::OnChannelPart( channel, user, message );
}

void TASServer::OnChannelTopic( const std::string& channel_name, const std::string& who, int /*unused*/, const std::string& message )
{
    ChannelPtr channel = m_channels.Get( "#" + channel_name );
	if(!channel) return;
    UserPtr user = m_users.FindByNick( who );
    if(!user) return;
    iServer::OnChannelTopic( channel, user,
                             boost::replace_all_copy( message, "\\n", "\n" ) );
}

void TASServer::OnChannelAction( const std::string& channel_name, const std::string& who, const std::string& action )
{
    ChannelPtr channel = m_channels.Get( "#" + channel_name );
    UserPtr user = m_users.FindByNick( who );
	if(!channel) return;
	if(!user) return;
    iServer::OnChannelAction( channel, user, action );
}

ChannelPtr TASServer::GetCreatePrivateChannel( const UserPtr user )
{
    if (!user) return ChannelPtr();
    std::string channame = "U" + Util::ToString(user->Id());
    ChannelPtr channel = m_channels.Get( channame );
	if (!channel)
	{
        channel = m_channels.Add( new Channel( channame ) );
        iServer::OnUserJoinedChannel( channel, user );
        iServer::OnUserJoinedChannel( channel, m_me );
	}
	return channel;
}

//! our own outgoing messages, user is destinatary
void TASServer::OnSayPrivateMessageEx( const std::string& user, const std::string& action )
{
    UserPtr usr = m_users.FindByNick( user );
	if(!usr) return;
    ChannelPtr channel = GetCreatePrivateChannel(usr);
    iServer::OnChannelAction( channel, m_me, action );
}

//! incoming messages, user is source
void TASServer::OnSaidPrivateMessageEx( const std::string& user, const std::string& action )
{
    UserPtr usr = m_users.FindByNick( user );
	if(!usr) return;
    ChannelPtr channel = GetCreatePrivateChannel(usr);
    iServer::OnChannelAction( channel, usr, action );
}

//! our own outgoing messages, user is destinatary
void TASServer::OnSayPrivateMessage( const std::string& user, const std::string& message )
{
    UserPtr usr = m_users.FindByNick( user );
	if(!usr) return;
    ChannelPtr channel = GetCreatePrivateChannel(usr);
    iServer::OnChannelSaid( channel, m_me, message );
}

//! incoming messages, user is source
void TASServer::OnSaidPrivateMessage( const std::string& user, const std::string& message )
{
    UserPtr usr = m_users.FindByNick( user );
	if(!usr) return;
    ChannelPtr channel = GetCreatePrivateChannel(usr);
    iServer::OnChannelSaid( channel, usr, message );
}

void TASServer::OnSaidBattle( const std::string& nick, const std::string& msg )
{
    UserPtr usr = m_users.FindByNick( nick );
	if(!usr) return;
    IBattlePtr battle = m_current_battle;
	if ( !battle ) return;
    ChannelPtr channel = battle->GetChannel();
	if (!channel) return;
    iServer::OnChannelSaid( channel, usr, msg );
}

void TASServer::OnBattleAction( const std::string& nick, const std::string& msg )
{
    UserPtr usr = m_users.FindByNick( nick );
	if(!usr) return;
    IBattlePtr battle = m_current_battle;
	if ( !battle ) return;
    ChannelPtr channel = battle->GetChannel();
	if (!channel) return;
    iServer::OnChannelSaid( channel, usr, msg );
}

void TASServer::OnBattleStartRectAdd( int allyno, int left, int top, int right, int bottom )
{
    IBattlePtr battle = m_current_battle;
	if ( !battle ) return;
    iServer::OnBattleStartRectAdd(battle, allyno, left, top, right, bottom);
}

void TASServer::OnBattleStartRectRemove( int allyno )
{
    IBattlePtr battle = m_current_battle;
	if ( !battle ) return;
    iServer::OnBattleStartRectRemove(battle, allyno);
}

void TASServer::OnScriptStart()
{
    IBattlePtr battle = m_current_battle;
	if ( !battle ) return;
	battle->ClearScript();
}

void TASServer::OnScriptLine( const std::string& line )
{
    IBattlePtr battle = m_current_battle;
	if ( !battle ) return;
	battle->AppendScriptLine( line );
}

void TASServer::OnScriptEnd()
{
    IBattlePtr battle = m_current_battle;
	if ( !battle ) return;
	OnBattleScript( battle, battle->GetScript() );

}

void TASServer::OnMutelistBegin( const std::string& channel )
{
	m_mutelist.clear();
    m_mutelist_current_channelname = channel;
}

void TASServer::OnMutelistItem( const std::string& mutee, const std::string& message )
{
    MuteListEntry entry( m_users.FindByNick( mutee ), message );
	m_mutelist.push_back(entry);
}

void TASServer::OnMutelistEnd()
{
    ChannelPtr chan = m_channels.Get("#" + m_mutelist_current_channelname);
    m_mutelist_current_channelname = "";
	if (!chan) return;
	OnMuteList(chan, m_mutelist);
}

void TASServer::OnChannelMessage( const std::string& channel, const std::string& msg )
{
    ChannelPtr chan = m_channels.Get(channel);
	if (!chan) return;
    iServer::OnChannelMessage( chan, msg );
}

void TASServer::OnRing( const std::string& from )
{
    iServer::OnRing(m_users.FindByNick(from));
}

void TASServer::OnKickedFromBattle()
{
    iServer::OnKickedFromBattle(m_current_battle);
    iServer::OnUserLeftBattle(m_current_battle,m_me);
}

void TASServer::OnKickedFromChannel( const std::string& channel, const std::string& fromWho, const std::string& message)
{
    ChannelPtr chan = m_channels.Get(channel);
	if(!chan) return;
    iServer::OnKickedFromChannel(chan, fromWho, message);
    iServer::OnUserLeftChannel(chan, m_me );
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
    UserPtr user = m_users.FindByNick( username );
	if (!user) return;

	OnUserIP( user, ip );
	OnUserExternalUdpPort( user, udpport );
}

void TASServer::OnHostExternalUdpPort( const int udpport )
{
    if (!m_current_battle) return;
    const UserPtr host = m_current_battle->GetFounder();
    OnUserExternalUdpPort(host, udpport);
    OnBattleHostchanged( m_current_battle, udpport );
}

void TASServer::OnChannelListEntry( const std::string& channel, const int& numusers, const std::string& topic )
{
    ChannelPtr chan = m_channels.Get( "#" + channel );
	if (!chan)
	{
        chan = m_channels.Add( new Channel( "#" + channel ) );
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
    iServer::OnRequestBattleStatus(m_current_battle);
}

void TASServer::OnBattleAddBot( int battleid, const std::string& nick, const std::string& owner, int intstatus, int intcolor, const std::string& aidll)
{
    BattlePtr battle = m_battles.Get(battleid);
	if (!battle) return;
	UTASBattleStatus tasbstatus;
	UserBattleStatus status;
	UTASColor color;
	tasbstatus.data =intstatus;
	status = ConvTasbattlestatus( tasbstatus.tasdata );
	color.data = intcolor;
    status.color = lslColor( color.color.red, color.color.green, color.color.blue );
	status.aishortname = aidll;
    status.owner = owner;
    IServerPtr iptr( this );
    UserPtr user( new User( nick, "", -1, iptr ) );
    battle->OnUserAdded( user );
    iServer::OnUserJoinedBattle( battle, user );
    iServer::OnUserBattleStatusUpdated( battle, user, status );
}

void TASServer::OnBattleUpdateBot( int battleid, const std::string& nick, int intstatus, int intcolor )
{
    BattlePtr battle = m_battles.Get(battleid);
	if (!battle) return;
	UTASBattleStatus tasbstatus;
	UserBattleStatus status;
	UTASColor color;
	tasbstatus.data =intstatus;
	status = ConvTasbattlestatus( tasbstatus.tasdata );
	color.data = intcolor;
    status.color = lslColor( color.color.red, color.color.green, color.color.blue );
    UserPtr user = battle->GetUser( nick );
    iServer::OnUserBattleStatusUpdated( battle, user, status );
}

void TASServer::OnBattleRemoveBot( int battleid, const std::string& nick )
{
    BattlePtr battle = m_battles.Get(battleid);
	if (!battle) return;
    UserPtr user = battle->GetUser( nick );
	if (!user ) return;
    iServer::OnUserLeftBattle( battle, user );
    if (user->BattleStatus().IsBot())
        iServer::OnUserQuit( user );
}

void TASServer::OnFileDownload( int intdata, const std::string& FileName, const std::string& url, const std::string& description )
{
	UTASOfferFileData parsingdata;
    parsingdata.data = intdata;
    iServer::OnFileDownload(  parsingdata.tasdata.autoopen, parsingdata.tasdata.closelobbyondownload,
                              parsingdata.tasdata.disconnectonrefuse, FileName, url, description );
}

void TASServer::SendCmd( const std::string& command, const boost::format& param )
{
	SendCmd( command, param.str() );
}

} // namespace LSL {
