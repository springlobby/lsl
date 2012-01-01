//SpringUnitSync& usync();

#include <unitsync++/c_api.h>
#include <unitsync++/image.h>
#include <unitsync++/unitsync.h>

void dummySync()
{
	   LSL::usync();
	   const bool usync_loaded = LSL::usync().ReloadUnitSyncLib();
	   LSL::UnitsyncImage mini = LSL::usync().GetMinimap( "Alaska" );
	   LSL::UnitsyncImage metal = LSL::usync().GetMetalmap( "Alaska" );
	   LSL::UnitsyncImage height = LSL::usync().GetHeightmap( "Alaska" );
	   mini.Save( "/tmp/alaska_mini.png" );
	   metal.Save( "/tmp/alaska_metal.png" );
	   height.Save( "/tmp/alaska_height.png" );
	   LSL::UnitsyncImage heightL( "/tmp/alaska_height.png" );
	   heightL.Save( "/tmp/alaska_heightL.png" );
}

