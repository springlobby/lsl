#ifndef LSL_HEADERGUARD_BATTLE_ENUM_H
#define LSL_HEADERGUARD_BATTLE_ENUM_H

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

enum HostInfo
{
	HI_None = 0,
	HI_Map = 1,
	HI_Locked = 2,
	HI_Spectators = 4,
	HI_StartResources = 8,
	HI_MaxUnits = 16,
	HI_StartType = 32,
	HI_GameType = 64,
	HI_Options = 128,
	HI_StartRects = 256,
	HI_Restrictions = 512,
	HI_Map_Changed = 1024,
	HI_Mod_Changed = 2048,
	HI_User_Positions  = 4096,
	HI_Send_All_opts  = 8192
};

const unsigned int DEFAULT_SERVER_PORT = 8452;
const unsigned int DEFAULT_EXTERNAL_UDP_SOURCE_PORT = 16941;

} //namespace Enum {
} //namespace LSL {

/**
 * \file enum.h
 * \section LICENSE
Copyright 2012 by The libSpringLobby team. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#endif // LSL_HEADERGUARD_BATTLE_ENUM_H
