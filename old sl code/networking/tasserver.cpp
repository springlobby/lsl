/* Copyright (C) 2007-2011 The SpringLobby Team. All rights reserved. */


#ifdef _MSC_VER
#ifndef NOMINMAX
	#define NOMINMAX
#endif // NOMINMAX
#include <winsock2.h>
#endif // _MSC_VER

#include <wx/string.h>
#include <wx/regex.h>
#include <wx/intl.h>
#include <wx/protocol/http.h>
#include <wx/socket.h>
#include <wx/log.h>
#include <wx/tokenzr.h>
#include <wx/platinfo.h>
#include <wx/stopwatch.h>

#include <stdexcept>
#include <algorithm>

#include <map>

#include "base64.h"
#include "utils/md5.h"
#include "tasserver.h"
#include "springunitsync.h"
#include "user.h"
#include "utils/debug.h"
#include "utils/tasutil.h"
#include "utils/conversion.h"
#include "utils/platform.h"
#include "updater/updatehelper.h"
#include "battle.h"
#include "serverevents.h"
#include "socket.h"
#include "channel/channel.h"
#include "tasservertokentable.h"
#include "pingthread.h"

// for SL_MAIN_ICON
#include "utils/customdialogs.h"

#include "settings.h"




















bool TASServer::ExecuteSayCommand( const wxString& cmd )
{

	else if ( subcmd == _T("/unmute") )
	{
				if ( arrayparams.GetCount() != 3 ) return false;
		SendCmd( _T("UNMUTE"), arrayparams[1] + _T(" ") + arrayparams[2] );
		return true;
	}
	else if ( subcmd == _T("/mutelist") )
	{
				if ( arrayparams.GetCount() != 2 ) return false;
		SendCmd( _T("MUTELIST"), arrayparams[1] );
		return true;
	}
	else if ( subcmd == _T("/lastlogin") )
	{
				if ( arrayparams.GetCount() != 2 ) return false;
		SendCmd( _T("GETLASTLOGINTIME"), arrayparams[1] );
		return true;
	}
	else if ( subcmd == _T("/findip") )
	{
				if ( arrayparams.GetCount() != 2 ) return false;
		SendCmd( _T("FINDIP"), arrayparams[1] );
		return true;
	}
	else if ( subcmd == _T("/lastip") )
	{
				if ( arrayparams.GetCount() != 2 ) return false;
		SendCmd( _T("GETLASTIP"), arrayparams[1] );
		return true;
	}
	else if ( subcmd == _T("/rename") )
	{
				if ( arrayparams.GetCount() != 2 ) return false;
		SendCmd( _T("RENAMEACCOUNT"), arrayparams[1] );
		sett().SetServerAccountNick( sett().GetDefaultServer(), arrayparams[1] ); // this code assumes that default server hasn't changed since login ( like it should atm )
		return true;
	}
	else if ( subcmd == _T("/testmd5") )
	{
		ExecuteCommand( _T("SERVERMSG"), GetPasswordHash(params) );
		return true;
	}
	else if ( subcmd == _T("/hook") )
	{
		SendCmd( _T("HOOK"), params );
		return true;
	}
	else if ( subcmd == _T("/quit") )
	{
		Disconnect();
		return true;
	}
	else if ( subcmd == _T("/changepassword2") )
	{
		if ( arrayparams.GetCount() < 1 ) return false;
		wxString oldpassword = sett().GetServerAccountPass( GetServerName() );
		wxString newpassword = GetPasswordHash( params );
		if  ( oldpassword.IsEmpty() || !sett().GetServerAccountSavePass(GetServerName()) )
		{
			m_se->OnServerMessage(_("There is no saved password for this account, please use /changepassword"));
			return true;
		}
		SendCmd( _T("CHANGEPASSWORD"), oldpassword + _T(" ") + newpassword );
		return true;
	}
	else if ( subcmd == _T("/changepassword") )
	{
		if ( arrayparams.GetCount() != 2 ) return false;
		wxString oldpassword = GetPasswordHash(arrayparams[1]);
		wxString newpassword = GetPasswordHash( arrayparams[2] );
		SendCmd( _T("CHANGEPASSWORD"), oldpassword + _T(" ") + newpassword );
		return true;
	}

}








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

	if ( cmd == "TASSERVER"))
	{
		mod = GetWordParam( params );
		mod.ToDouble( &m_ser_ver );
		supported_spring_version = GetWordParam( params );
		m_nat_helper_port = (unsigned long)GetIntParam( params );
		lanmode = GetBoolParam( params );
		m_server_lanmode = lanmode;
		m_se->OnConnected( m_server_name, mod, (m_ser_ver > 0), supported_spring_version, lanmode );
	}
	else if ( cmd == "ACCEPTED") )
	{
		if ( m_online ) return; // in case is the server sends WTF
		m_online = true;
		m_user = params;
		m_ping_thread = new PingThread( *this, 10000 );
		m_ping_thread->Init();
		m_se->OnLogin( );
	}
	else if ( cmd == "MOTD") )
	{
		m_se->OnMotd( params );
	}
	else if ( cmd == "ADDUSER") )
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
		m_se->OnNewUser( nick, contry, cpu, Tostd::string(id) );
		if ( nick == m_relay_host_bot )
		{
		   RelayCmd( "OPENBATTLE"), m_delayed_open_command ); // relay bot is deployed, send host command
		   m_delayed_open_command = "");
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
