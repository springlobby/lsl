#ifndef LSL_HEADERGUARD_BATTLE_H
#define LSL_HEADERGUARD_BATTLE_H

/** \file battle.h
    \copyright GPL v2 **/

#include <set>

#include <lslutils/type_forwards.h>
#include "ibattle.h"
#include "enum.h"

namespace LSL {

class Server;
class User;
class UnitsyncMap;
class UnitsyncMod;

namespace Battle {

/** \brief model of a multiplayer battle
* \todo DOCME */
class Battle : public IBattle
{
public:
    Battle(IServerPtr serv, int id );
    ~Battle();
    int key() const { return m_id; }

    const IServerPtr GetServer() { return m_serv; }
    const ConstIServerPtr GetServer() const { return m_serv; }

		void SendHostInfo( Enum::HostInfo update );
    void SendHostInfo( const std::string& Tag );

    void Update();
    void Update( const std::string& Tag );

    void Join( const std::string& password = "" );
    void Leave();

    void KickPlayer( const UserPtr user );

    void RingNotReadyPlayers();
    void RingNotSyncedPlayers();
    void RingNotSyncedAndNotReadyPlayers();
    void RingPlayer( const ConstUserPtr u );

    void Say( const std::string& msg );
    void DoAction( const std::string& msg );

    void SetLocalMap( const UnitsyncMap& map );

    void OnRequestBattleStatus();
    void SendMyBattleStatus();

    bool ExecuteSayCommand( const std::string& cmd );

    void AddBot( const std::string& nick, UserBattleStatus status );

    void ForceSide( const UserPtr user, int side );
    void ForceTeam( const UserPtr user, int team );
    void ForceAlly( const UserPtr user, int ally );
    void ForceColor( const UserPtr user, const lslColor& col );
    void ForceSpectator( const UserPtr user, bool spectator );
    void BattleKickPlayer( const UserPtr user );
    void SetHandicap( const UserPtr user, int handicap);

    void OnUserAdded( const UserPtr user );
    void OnUserBattleStatusUpdated( const UserPtr user, UserBattleStatus status );
    void OnUserRemoved( const UserPtr user );

    void ForceUnsyncedToSpectate();
    void ForceUnReadyToSpectate();
    void ForceUnsyncedAndUnreadyToSpectate();

    void SetAutoLockOnStart( bool value );
    bool GetAutoLockOnStart();

    void SetLockExternalBalanceChanges( bool value );
    bool GetLockExternalBalanceChanges();

    void FixColors();
    void Autobalance( Enum::BalanceType balance_type = Enum::balance_divide, bool clans = true, bool strong_clans = true, int allyteamsize = 0 );
    void FixTeamIDs( Enum::BalanceType balance_type = Enum::balance_divide, bool clans = true, bool strong_clans = true, int controlteamsize = 0 );

    void SendScriptToClients();

    ///< quick hotfix for bans
    bool IsBanned(const UserPtr user );
    ///>

    void SetImReady( bool ready );

    UserPtr GetMe();
    const ConstUserPtr GetMe() const;

    void UserPositionChanged( const UserPtr user );

    void SaveMapDefaults();
    void LoadMapDefaults( const std::string& mapname );

    void StartHostedBattle();
    void StartSpring();

    void SetInGame( bool ingame );

    void OnUnitsyncReloaded(  );

    void SetAutoUnspec(bool value);
    bool GetAutoUnspec() { return m_auto_unspec; }

    void ShouldAutoUnspec();
    void SetChannel( const ChannelPtr channel );
    const ChannelPtr GetChannel();
private:
    void OnTimer( const boost::system::error_code& error );
    // Battle variables

    ///< quick hotfix for bans
    std::set<std::string> m_banned_users;
    std::set<std::string> m_banned_ips;
    ///>

    IServerPtr m_serv;
    bool m_autolock_on_start;
    bool m_auto_unspec;
    const int m_id;
    ChannelPtr m_channel;
};

} // namespace Battle {
} // namespace LSL {

#endif // LSL_HEADERGUARD_BATTLE_H

/**
This file is part of SpringLobby,
Copyright (C) 2007-2011

SpringLobby is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

SpringLobby is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SpringLobby.  If not, see <http://www.gnu.org/licenses/>.
**/

