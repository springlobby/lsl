#ifndef LSL_BATTLE_ENUM_H
#define LSL_BATTLE_ENUM_H

namespace LSL {
namespace Enum {

enum NatType
{
    NAT_None = 0,
    NAT_Hole_punching,
    NAT_Fixed_source_ports
};


enum BattleType
{
    BT_Played,
    BT_Replay,
    BT_Savegame
};

enum BalanceType
{
    balance_divide,
    balance_random
};

enum StartType
{
    ST_Fixed = 0,
    ST_Random = 1,
    ST_Choose = 2,
    ST_Pick = 3
};

enum GameType
{
    GT_ComContinue = 0,
    GT_ComEnds = 1,
    GT_Lineage = 2
};

const unsigned int DEFAULT_SERVER_PORT = 8452;
const unsigned int DEFAULT_EXTERNAL_UDP_SOURCE_PORT = 16941;

} //namespace Enum {
} //namespace LSL {

#endif // LSL_BATTLE_ENUM_H
