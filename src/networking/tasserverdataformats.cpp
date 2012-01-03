#include "tasserverdataformats.h"
#include <user/user.h>
#include <battle/ibattle.h>
#include <utils/debug.h>

#define ASSERT_EXCEPTION(cond,msg) do { if (!(cond)) { LSL_THROW( conversion, msg ); } } while (0)

////////////////////////
// Utility functions
//////////////////////
namespace LSL {

UserStatus ConvTasclientstatus( TASClientstatus tas )
{
    UserStatus stat;
    stat.in_game = tas.in_game;
    stat.away = tas.away;
    stat.rank = (UserStatus::RankContainer)tas.rank;
    stat.moderator = tas.moderator;
    stat.bot = tas.bot;
    return stat;
}

UserBattleStatus ConvTasbattlestatus( TASBattleStatus tas )
{
    UserBattleStatus stat;
    stat.ally = tas.ally;
    stat.handicap = tas.handicap;
    stat.ready = (tas.ready==1)?true:false;
    stat.side = tas.side;
    stat.spectator = (tas.player == 0)?true:false;
    stat.sync = tas.sync;
    stat.team = tas.team;
    return stat;
}


TASBattleStatus ConvTasbattlestatus( UserBattleStatus bs)
{
    TASBattleStatus stat;
    stat.ally = bs.ally;
    stat.handicap = bs.handicap;
    stat.ready = bs.ready?1:0;
    stat.side = bs.side;
    stat.player = bs.spectator?0:1;
    stat.sync = bs.sync;
    stat.team = bs.team;
    return stat;
}


Battle::StartType IntToStartType( int start )
{
    switch ( start )
    {
    case 0:
		return Battle::ST_Fixed;
    case 1:
		return Battle::ST_Random;
    case 2:
		return Battle::ST_Choose;
    default:
		ASSERT_EXCEPTION( false, "int to IBattle::StartType" );
    };
	return Battle::ST_Fixed;
}


Battle::NatType IntToNatType( int nat )
{
    switch ( nat )
    {
    case 0:
		return Battle::NAT_None;
    case 1:
		return Battle::NAT_Hole_punching;
    case 2:
		return Battle::NAT_Fixed_source_ports;
    default:
		ASSERT_EXCEPTION( false, "int to NatType" );
    };
	return Battle::NAT_None;
}


Battle::GameType IntToGameType( int gt )
{
    switch ( gt )
    {
    case 0:
		return Battle::GT_ComContinue;
    case 1:
		return Battle::GT_ComEnds;
    case 2:
		return Battle::GT_Lineage;
    default:
		ASSERT_EXCEPTION( false, "int to IBattle::GameType" );
    };
	return Battle::GT_ComContinue;
}

} // namespace LSL {
