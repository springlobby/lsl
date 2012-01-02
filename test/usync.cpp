//SpringUnitSync& usync();

#include <unitsync++/c_api.h>
#include <unitsync++/image.h>
#include <unitsync++/unitsync.h>
#include <boost/timer/timer.hpp>
#include <boost/format.hpp>
#include <cmath>

void dummySync()
{
	LSL::Unitsync usync;
	{
		boost::timer::auto_cpu_timer t;
		const bool usync_loaded = usync.ReloadUnitSyncLib();
	}
	std::cout << boost::format( "found %d maps and %d games\n") % usync.GetNumMaps() % usync.GetNumMods() ;
	LSL::UnitsyncImage mini = usync.GetMinimap( "Alaska" );
	LSL::UnitsyncImage metal = usync.GetMetalmap( "Alaska" );
	LSL::UnitsyncImage height = usync.GetHeightmap( "Alaska" );
	mini.Save( "/tmp/alaska_mini.png" );
	metal.Save( "/tmp/alaska_metal.png" );
	height.Save( "/tmp/alaska_height.png" );
	LSL::UnitsyncImage heightL( "/tmp/alaska_height.png" );
	heightL.Save( "/tmp/alaska_heightL.png" );
}

