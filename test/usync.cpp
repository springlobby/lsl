//SpringUnitSync& usync();

#include <unitsync++/c_api.h>

void dummySync()
{
	   LSL::SpringUnitSyncLib s;
	   s.Load( "/usr/lib/spring/libunitsync.so", "" );
//	   s.GetMinimap( "Alaska" );
}

