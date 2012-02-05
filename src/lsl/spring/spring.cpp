/* Copyright (C) 2007 The SpringLobby Team. All rights reserved. */
//
// Class: Spring
//

#ifdef _MSC_VER
#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX
#include <winsock2.h>
#endif // _MSC_VER

#include <stdexcept>
#include <vector>
#include <algorithm>
#include <fstream>
#include <clocale>

#include "spring.h"
#include "springprocess.h"
#include <lslutils/debug.h>
#include <lslutils/conversion.h>
#include <lslutils/lslconfig.h>
#include <lsl/container/userlist.h>
#include <lsl/battle/battle.h>
#include <lsl/battle/singleplayer.h>
#include <lsl/battle/offline.h>
#include <lsl/user/user.h>
#include <unitsync++/unitsync.h>
#include <lsl/battle/tdfcontainer.h>

#include <lslutils/globalsmanager.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace LSL {

namespace BF = boost::filesystem;

Spring& spring()
{
    static LineInfo<Spring> m( AT );
    static GlobalObjectHolder<Spring,LineInfo<Spring> > m_spring( m );
    return m_spring;
}

Spring::Spring()
    : m_process(0),
      m_running(false)
{}

Spring::~Spring()
{
    delete m_process;
}

bool Spring::IsRunning() const
{
    return m_running;
}

bool Spring::RunReplay ( const std::string& filename )
{
    LslDebug( "launching spring with replay: %s",  filename.c_str() );

    return LaunchSpring( "\"" + filename + "\"");
}

bool Spring::Run( const BattlePtr battle )
{
    BF::path path = sett().GetCurrentUsedDataDir();
    path /= "script.txt";
    try {
        BF::ofstream f( path );
        if ( !f.is_open() ) {
            LslError( "Access denied to script.txt at %s", path.string().c_str() );
        }
        battle->DisableHostStatusInProxyMode( true );
        f << WriteScriptTxt(battle);
        battle->DisableHostStatusInProxyMode( false );
        f.close();
    }
    catch ( std::exception& e ) {
        LslError( "Couldn't write script.txt, exception caught:\n %s", e.what() );
        return false;
    }
    catch (...) {
        LslError( "Couldn't write script.txt" );
        return false;
    }

    std::string cmd;
    //! TODO
//    if ( battle->GetAutoHost().GetEnabled() )
//    {
//        // -m, --minimise          Start minimised
//        // -q [T], --quit=[T]      Quit immediately on game over or after T seconds
//        cmd = _T("--minimise");
//    }
    cmd += std::string(" \"" + path.string() +  "\"");
    return LaunchSpring( cmd );
}

//bool Spring::Run( Battle::SinglePlayerBattle& battle )
//{
//    BF::path path = sett().GetCurrentUsedDataDir();
//    path /= "script.txt";
//    std::string cmd = "\"" + path.string().c_str() + "\"";
//    try {
//        BF::ofstream f( path );
//        if ( !f.is_open() ) {
//            LslError( "Access denied to script.txt at %s", path.string().c_str() );
//        }
//        battle->DisableHostStatusInProxyMode( true );
//        f << WriteScriptTxt(battle);
//        battle->DisableHostStatusInProxyMode( false );
//        f.close();
//    }
//    catch ( std::exception& e ) {
//        LslError( "Couldn't write script.txt, exception caught:\n %s", e.what() );
//        return false;
//    }
//    catch (...) {
//        LslError( "Couldn't write script.txt" );
//        return false;
//    }
//    return LaunchSpring( cmd );
//}

//bool Spring::Run(NoGuiSinglePlayerBattle &battle)
//{
//    BF::path path = sett().GetCurrentUsedDataDir();
//    path /= "script.txt";
//    std::string cmd = "\"" + path.string().c_str() + "\"";
//    try {
//        BF::ofstream f( path );
//        if ( !f.is_open() ) {
//            LslError( "Access denied to script.txt at %s", path.string().c_str() );
//        }
//        battle->DisableHostStatusInProxyMode( true );
//        f << WriteScriptTxt(battle);
//        battle->DisableHostStatusInProxyMode( false );
//        f.close();
//    }
//    catch ( std::exception& e ) {
//        LslError( "Couldn't write script.txt, exception caught:\n %s", e.what() );
//        return false;
//    }
//    catch (...) {
//        LslError( "Couldn't write script.txt" );
//        return false;
//    }
//    return LaunchSpring( cmd );
//}

bool Spring::Run(const std::string& script)
{
    BF::path path = sett().GetCurrentUsedDataDir();
    path /= "script.txt";
    std::string cmd = std::string(" \"" + path.string() +  "\"");
    try {
        BF::ofstream f( path );
        if ( !f.is_open() ) {
            LslError( "Access denied to script.txt at %s", path.string().c_str() );
        }
        f << script;
        f.close();
    }
    catch ( std::exception& e ) {
        LslError( "Couldn't write script.txt, exception caught:\n %s", e.what() );
        return false;
    }
    catch (...) {
        LslError( "Couldn't write script.txt" );
        return false;
    }
    return LaunchSpring( cmd );
}

bool Spring::Run( Battle::OfflineBattle& battle )
{
    std::string path = battle.GetPlayBackFilePath();
    return LaunchSpring( "\"" + path + "\"" );
}

bool Spring::LaunchSpring( const std::string& params  )
{
    if ( m_running )
    {
        LslError( "Spring already running!" );
        return false;
    }
    if ( !Util::FileExists( sett().GetCurrentUsedSpringBinary() ) ) {
        LslError( "spring binary not found at set location: %s", sett().GetCurrentUsedSpringBinary().c_str() );
        return false;
    }

    std::string configfileflags = sett().GetCurrentUsedSpringConfigFilePath();
    if ( !configfileflags.empty() )
    {
        configfileflags = "--config=\"" + configfileflags + "\" ";
    }

    std::string cmd =  "\"" + sett().GetCurrentUsedSpringBinary();
#ifdef __WXMAC__
    wxChar sep = wxFileName::GetPathSeparator();
    if ( sett().GetCurrentUsedSpringBinary().AfterLast(_T('.')) == _T("app") )
        cmd += sep + std::string(_T("Contents")) + sep + std::string(_T("MacOS")) + sep + std::string(_T("spring")); // append app bundle inner path
#endif
    cmd += "\" " + configfileflags + params;

    LslDebug( "spring call params: %s", cmd.c_str() );
    if ( m_process == 0 )
        m_process = new SpringProcess( *this );
    m_process->Create();
    m_process->SetCommand( cmd );
    m_process->Run();

    m_running = true;
    return true;
}


void Spring::OnTerminated(int event )
{
    m_running = false;

    int exit_code = 0l;
    sig_springStopped(exit_code,"");
}


std::string Spring::WriteScriptTxt( const ConstIBattlePtr battle ) const
{
    std::stringstream ret;

    TDF::TDFWriter tdf(ret);

    // Start generating the script.
    tdf.EnterSection( _T("GAME") );

    tdf.Append( _T("HostIP"), battleGetHostIp() );
    if ( battle->IsFounderMe() )
    {
        if ( battle->GetNatType() == NAT_Hole_punching ) tdf.Append( _T("HostPort"), battle->GetMyInternalUdpSourcePort() );
        else tdf.Append(_T("HostPort"), battle->GetHostPort() );
    }
    else
    {
        tdf.Append( _T("HostPort"), battle->GetHostPort() );
        if ( battle->GetNatType() == NAT_Hole_punching )
        {
            tdf.Append( _T("SourcePort"), battle->GetMyInternalUdpSourcePort() );
        }
        else if ( sett().GetClientPort() != 0)
        {
            tdf.Append( _T("SourcePort"), sett().GetClientPort() ); /// this allows to play with broken router by setting SourcePort to some forwarded port.
        }
    }
    tdf.Append( _T("IsHost"), battle->IsFounderMe() );

    const ConstUserPtr  me = battle->GetMe();
    tdf.Append(_T("MyPlayerName"), me.Nick() );

    if ( !me.BattleStatus().scriptPassword.empty() )
    {
        tdf.Append( _T("MyPasswd"), me.BattleStatus().scriptPassword );
    }

    if ( !battle->IsFounderMe() )
    {
        tdf.LeaveSection();
        return ret;
    }

    /**********************************************************************************
                                                                        Host-only section
            **********************************************************************************/

    tdf.AppendLineBreak();

    tdf.Append(_T("ModHash"), battle->LoadMod().hash );
    tdf.Append(_T("MapHash"), battle->LoadMap().hash );

    tdf.Append( _T("Mapname"), battle->GetHostMapName() );
    tdf.Append( _T("GameType"), battle->GetHostModName() );

    tdf.AppendLineBreak();

    switch ( battle->GetBattleType() )
    {
    case BT_Played: break;
    case BT_Replay:
    {
        std::string path = battle->GetPlayBackFilePath();
        if ( path.Find(_T("/")) != wxNOT_FOUND ) path.BeforeLast(_T('/'));
        tdf.Append( _T("DemoFile"), path );
        tdf.AppendLineBreak();
        break;
    }
    case BT_Savegame:
    {
        std::string path = battle->GetPlayBackFilePath();
        if ( path.Find(_T("/")) != wxNOT_FOUND ) path.BeforeLast(_T('/'));
        tdf.Append( _T("Savefile"), path );
        tdf.AppendLineBreak();
        break;
    }
    default:
        wxLogDebugFunc( _T("") ); break;
    }

    long startpostype;
    battle->CustomBattleOptions().getSingleValue( _T("startpostype"), OptionsWrapper::EngineOption ).ToLong( &startpostype );

    std::vector<StartPos> remap_positions;
    if ( battle->IsProxy() && ( startpostype != IBattle::ST_Pick ) && ( startpostype != IBattle::ST_Choose ) )
    {
        std::set<int> parsedteams;
        unsigned int NumUsers = battle->GetNumUsers();
        unsigned int NumTeams = 0;
        for ( unsigned int i = 0; i < NumUsers; i++ )
        {
            const ConstUserPtr  usr = battle->GetUser( i );
            UserBattleStatus& status = usr.BattleStatus();
            if ( status.spectator ) continue;
            if ( parsedteams.find( status.team ) != parsedteams.end() ) continue; // skip duplicates
            parsedteams.insert( status.team );
            NumTeams++;
        }

        MapInfo infos = battle->LoadMap().info;
        unsigned int nummapstartpositions = infos.positions.size();
        unsigned int copysize = std::min( nummapstartpositions, NumTeams );
        remap_positions = std::vector<StartPos> ( infos.positions.begin(), infos.positions.begin() + copysize ); // only add the first x positions

        if ( startpostype == IBattle::ST_Random )
        {
            random_shuffle( remap_positions.begin(), remap_positions.end() ); // shuffle the positions
        }

    }
    if ( battle->IsProxy() )
    {
        if ( ( startpostype == IBattle::ST_Random ) || ( startpostype == IBattle::ST_Fixed ) )
        {
            tdf.Append( _T("startpostype"), IBattle::ST_Pick );
        }
        else tdf.Append( _T("startpostype"), startpostype );
    }
    else tdf.Append( _T("startpostype"), startpostype );

    tdf.EnterSection( _T("mapoptions") );
    OptionsWrapper::std::stringTripleVec optlistMap = battle->CustomBattleOptions().getOptions( OptionsWrapper::MapOption );
    for (OptionsWrapper::std::stringTripleVec::const_iterator it = optlistMap.begin(); it != optlistMap.end(); ++it)
    {
        tdf.Append(it->first,it->second.second);
    }
    tdf.LeaveSection();


    tdf.EnterSection(_T("modoptions"));
    tdf.Append( _T("relayhoststartpostype"), startpostype ); // also save the original wanted setting
    OptionsWrapper::std::stringTripleVec optlistMod = battle->CustomBattleOptions().getOptions( OptionsWrapper::ModOption );
    for (OptionsWrapper::std::stringTripleVec::const_iterator it = optlistMod.begin(); it != optlistMod.end(); ++it)
    {
        tdf.Append(it->first,it->second.second);
    }
    tdf.LeaveSection();

    std::map<std::string,int> units = battle->RestrictedUnits();
    tdf.Append( _T("NumRestrictions"), units.size());
    tdf.EnterSection( _T("RESTRICT") );
    int restrictcount = 0;
    for ( std::map<std::string, int>::const_iterator itor = units.begin(); itor != units.end(); itor++ )
    {
        tdf.Append(_T("Unit") + Tostd::string( restrictcount ), itor->first );
        tdf.Append(_T("Limit") + Tostd::string( restrictcount ), itor->second );
        restrictcount++;
    }
    tdf.LeaveSection();


    tdf.AppendLineBreak();

    if ( battle->IsProxy() )
    {
        tdf.Append( _T("NumPlayers"), battle->GetNumPlayers() -1 );
        tdf.Append( _T("NumUsers"), battle->GetNumUsers() -1 );
    }
    else
    {
        tdf.Append( _T("NumPlayers"), battle->GetNumPlayers() );
        tdf.Append( _T("NumUsers"), battle->GetNumUsers() );
    }

    tdf.AppendLineBreak();

    unsigned int NumUsers = battle->GetNumUsers();

    typedef std::map<int, int> ProgressiveTeamsVec;
    typedef ProgressiveTeamsVec::iterator ProgressiveTeamsVecIter;
    ProgressiveTeamsVec teams_to_sorted_teams; // original team -> progressive team
    int free_team = 0;
    std::map<const ConstUserPtr, int> player_to_number; // player -> ordernumber
    srand ( time(NULL) );
    for ( unsigned int i = 0; i < NumUsers; i++ )
    {
        const ConstUserPtr  user = battle->GetUser( i );
        UserBattleStatus& status = user.BattleStatus();
        if ( !status.spectator )
        {
            ProgressiveTeamsVecIter itor = teams_to_sorted_teams.find ( status.team );
            if ( itor == teams_to_sorted_teams.end() )
            {
                teams_to_sorted_teams[status.team] = free_team;
                free_team++;
            }
        }
        if ( battle->IsProxy() && ( user.Nick() == battle->GetFounder().Nick() ) ) continue;
        if ( status.IsBot() ) continue;
        tdf.EnterSection( _T("PLAYER") + Tostd::string( i ) );
        tdf.Append( _T("Name"), user.Nick() );
        tdf.Append( _T("CountryCode"), user.GetCountry().Lower());
        tdf.Append( _T("Spectator"), status.spectator );
        tdf.Append( _T("Rank"), (int)user.GetRank() );
        tdf.Append( _T("IsFromDemo"), int(status.isfromdemo) );
        if ( !status.scriptPassword.empty() )
        {
            tdf.Append( _T("Password"), status.scriptPassword );
        }
        if ( !status.spectator )
        {
            tdf.Append( _T("Team"), teams_to_sorted_teams[status.team] );
        }
        else
        {
            int speccteam = 0;
            if ( teams_to_sorted_teams.size() != 0 ) speccteam = rand() % teams_to_sorted_teams.size();
            tdf.Append( _T("Team"), speccteam );
        }
        tdf.LeaveSection();
        player_to_number[&user] = i;
    }
    if ( usync().VersionSupports( SpringUnitSync::USYNC_GetSkirmishAI ) )
    {
        for ( unsigned int i = 0; i < NumUsers; i++ )
        {
            const ConstUserPtr  user = battle->GetUser( i );
            UserBattleStatus& status = user.BattleStatus();
            if ( !status.IsBot() ) continue;
            tdf.EnterSection( _T("AI") + Tostd::string( i ) );
            tdf.Append( _T("Name"), user.Nick() ); // AI's nick;
            tdf.Append( _T("ShortName"), status.aishortname ); // AI libtype
            tdf.Append( _T("Version"), status.aiversion ); // AI libtype version
            tdf.Append( _T("Team"), teams_to_sorted_teams[status.team] );
            tdf.Append( _T("IsFromDemo"), int(status.isfromdemo) );
            tdf.Append( _T("Host"), player_to_number[battle->GetUser( status.owner )] );
            tdf.EnterSection( _T("Options") );
            int optionmapindex = battle->CustomBattleOptions().GetAIOptionIndex( user.Nick() );
            if ( optionmapindex > 0 )
            {
                OptionsWrapper::std::stringTripleVec optlistMod_ = battle->CustomBattleOptions().getOptions( (OptionsWrapper::GameOption)optionmapindex );
                for (OptionsWrapper::std::stringTripleVec::const_iterator it = optlistMod_.begin(); it != optlistMod_.end(); ++it)
                {
                    tdf.Append(it->first,it->second.second);
                }
            }
            tdf.LeaveSection();
            tdf.LeaveSection();
            player_to_number[&user] = i;
        }
    }

    tdf.AppendLineBreak();

    std::set<int> parsedteams;
    StringVector sides = usync().GetSides( battle->GetHostModName() );
    for ( unsigned int i = 0; i < NumUsers; i++ )
    {
        const ConstUserPtr  usr = battle->GetUser( i );
        UserBattleStatus& status = usr.BattleStatus();
        if ( status.spectator ) continue;
        if ( parsedteams.find( status.team ) != parsedteams.end() ) continue; // skip duplicates
        parsedteams.insert( status.team );

        tdf.EnterSection( _T("TEAM") + Tostd::string( teams_to_sorted_teams[status.team] ) );
        if ( !usync().VersionSupports( SpringUnitSync::USYNC_GetSkirmishAI ) && status.IsBot() )
        {
            tdf.Append( _T("AIDLL"), status.aishortname );
            tdf.Append( _T("TeamLeader"), player_to_number[battle->GetUser( status.owner )] ); // bot owner is the team leader
        }
        else
        {
            if ( status.IsBot() )
            {
                tdf.Append( _T("TeamLeader"), player_to_number[battleGetUser( status.owner )] );
            }
            else
            {
                tdf.Append( _T("TeamLeader"), player_to_number[&usr] );
            }
        }
        if ( battle->IsProxy() )
        {
            if ( startpostype == IBattle::ST_Pick )
            {
                tdf.Append(_T("StartPosX"), status.pos.x );
                tdf.Append(_T("StartPosZ"), status.pos.y );
            }
            else if ( ( startpostype == IBattle::ST_Fixed ) || ( startpostype == IBattle::ST_Random ) )
            {
                int teamnumber = teams_to_sorted_teams[status.team];
                if ( teamnumber < int(remap_positions.size()) ) // don't overflow
                {
                    StartPos position = remap_positions[teamnumber];
                    tdf.Append(_T("StartPosX"), position.x );
                    tdf.Append(_T("StartPosZ"), position.y );
                }
            }
        }
        else
        {
            if ( startpostype == IBattle::ST_Pick )
            {
                tdf.Append(_T("StartPosX"), status.pos.x );
                tdf.Append(_T("StartPosZ"), status.pos.y );
            }
        }

        tdf.Append( _T("AllyTeam"),status.ally );

        std::string colorstring =
                Tostd::string( status.color.Red()/255.0 ) + _T(' ') +
                Tostd::string( status.color.Green()/255.0 ) + _T(' ') +
                Tostd::string( status.color.Blue()/255.0 );
        tdf.Append( _T("RGBColor"), colorstring);

        unsigned int side = status.side;
        if ( side < sides.GetCount() ) tdf.Append( _T("Side"), sides[side] );
        tdf.Append( _T("Handicap"), status.handicap );
        tdf.LeaveSection();
    }

    tdf.AppendLineBreak();

    unsigned int maxiter = std::max( NumUsers, battleGetLastRectIdx() + 1 );
    std::set<int> parsedallys;
    for ( unsigned int i = 0; i < maxiter; i++ )
    {

        const ConstUserPtr  usr = battleGetUser( i );
        UserBattleStatus& status = usr.BattleStatus();
        BattleStartRect sr = battleGetStartRect( i );
        if ( status.spectator && !sr.IsOk() )
            continue;
        int ally = status.ally;
        if ( status.spectator )
            ally = i;
        if ( parsedallys.find( ally ) != parsedallys.end() )
            continue; // skip duplicates
        sr = battleGetStartRect( ally );
        parsedallys.insert( ally );

        tdf.EnterSection( _T("ALLYTEAM") + Tostd::string( ally ) );
        tdf.Append( _T("NumAllies"), 0 );
        if ( startpostype == IBattle::ST_Choose )
        {
            if ( sr.IsOk() )
            {
                const char* old_locale = std::setlocale(LC_NUMERIC, "C");

                tdf.Append( _T("StartRectLeft"), wxFormat( _T("%.3f") ) % ( sr.left / 200.0 ) );
                tdf.Append( _T("StartRectTop"), wxFormat( _T("%.3f") ) % ( sr.top / 200.0 ) );
                tdf.Append( _T("StartRectRight"), wxFormat( _T("%.3f") ) % ( sr.right / 200.0 ) );
                tdf.Append( _T("StartRectBottom"), wxFormat( _T("%.3f") ) % ( sr.bottom / 200.0 ) );

                std::setlocale(LC_NUMERIC, old_locale);
            }
        }
        tdf.LeaveSection();
    }

    tdf.LeaveSection();

    return ret;

}

} // namespace LSL {
