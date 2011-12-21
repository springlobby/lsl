#include "iserver.h"

iServer::iServer():
m_keepalive(15),
m_ping_timeout(40)
{
}


iServer::~iServer()
{
}





void iServer::OnDisconnected()
{
	// delete all users, battles, channels
}

void iServer::RequestSpringUpdate()
{
}

UserVector iServer::GetRelayHostList()
{
	if ( UserExists( "RelayHostManagerList" ) )
	{
		SayPrivate( "RelayHostManagerList", "!listmanagers" );
	}
	UserVector ret;
	for ( unsigned int i = 0; i < m_relay_host_manager_list.count(); i++ )
	{
		User* manager = GetUser( m_relay_host_manager_list[i] );
		// skip the manager is not connected or reports it's ingame ( no slots available ), or it's away ( functionality disabled )
		if (!manager) continue;
		if ( manager->Status().in_game ) continue;
		if ( manager->Status().away ) continue;
		ret.push_back( manager );
	}
	return ret;
}
