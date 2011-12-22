#include "tasserverdataformats.h"
////////////////////////
// Utility functions
//////////////////////

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


IBattle::StartType IntToStartType( int start )
{
    switch ( start )
    {
    case 0:
        return IBattle::ST_Fixed;
    case 1:
        return IBattle::ST_Random;
    case 2:
        return IBattle::ST_Choose;
    default:
        ASSERT_EXCEPTION( false, _T("invalid value") );
    };
    return IBattle::ST_Fixed;
}


NatType IntToNatType( int nat )
{
    switch ( nat )
    {
    case 0:
        return NAT_None;
    case 1:
        return NAT_Hole_punching;
    case 2:
        return NAT_Fixed_source_ports;
    default:
        ASSERT_EXCEPTION( false, _T("invalid value") );
    };
    return NAT_None;
}


IBattle::GameType IntToGameType( int gt )
{
    switch ( gt )
    {
    case 0:
        return IBattle::GT_ComContinue;
    case 1:
        return IBattle::GT_ComEnds;
    case 2:
        return IBattle::GT_Lineage;
    default:
        ASSERT_EXCEPTION( false, _T("invalid value") );
    };
    return IBattle::GT_ComContinue;
}
