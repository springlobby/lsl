#include "common.h"

namespace LSL {

void CommonUser::UpdateBattleStatus( const UserBattleStatus& status )
{
	// total 17 members to update.
	m_bstatus.team = status.team;
	m_bstatus.ally = status.ally;
	m_bstatus.color = status.color;
	m_bstatus.color_index = status.color_index;
	m_bstatus.handicap = status.handicap;
	m_bstatus.side = status.side;
	m_bstatus.sync = status.sync;
	m_bstatus.spectator = status.spectator;
	m_bstatus.ready = status.ready;
	if( !status.aishortname.empty() ) m_bstatus.aishortname = status.aishortname;
	if( !status.airawname.empty() ) m_bstatus.airawname = status.airawname;
	if( !status.aiversion.empty() ) m_bstatus.aiversion = status.aiversion;
	if( !status.aitype > 0 ) m_bstatus.aitype = status.aitype;
	if( !status.owner.empty() ) m_bstatus.owner = status.owner;
	if( status.pos.x > 0 ) m_bstatus.pos.x = status.pos.x;
	if( status.pos.y > 0 ) m_bstatus.pos.y = status.pos.y;

	// update ip and port if those were set.
	if( !status.ip.empty() ) m_bstatus.ip = status.ip;
	if( status.udpport != 0 ) m_bstatus.udpport = status.udpport;// 15
}

void CommonUser::SetStatus( const UserStatus& status )
{
	m_status = status;
}

} //namespace LSL {
