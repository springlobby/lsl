//SpringUnitSync& usync();

#include <unitsync++/c_api.h>
#include <unitsync++/image.h>

void dummySync()
{
	   LSL::UnitsyncLib s;
	   s.Load( "/usr/lib/spring/libunitsync.so", "" );
	   LSL::UnitsyncImage mini = s.GetMinimap( "Alaska" );
	   LSL::UnitsyncImage metal = s.GetMetalmap( "Alaska" );
	   LSL::UnitsyncImage height = s.GetHeightmap( "Alaska" );
	   mini.Save( "/tmp/alaska_mini.png" );
	   metal.Save( "/tmp/alaska_metal.png" );
	   height.Save( "/tmp/alaska_height.png" );
	   LSL::UnitsyncImage heightL( "/tmp/alaska_height.png" );
	   heightL.Save( "/tmp/alaska_heightL.png" );
}

