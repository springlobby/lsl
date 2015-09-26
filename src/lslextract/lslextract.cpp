#include "lslunitsync/unitsync.h"
#include "lslunitsync/image.h"
#include "lslutils/logging.h"
#include "lslutils/type_forwards.h"
#include "lslutils/config.h"
#include <stdarg.h>
#include <stdio.h>

void lsllogerror(const char* format, ...)
{
	va_list args;
	va_start (args, format);
	vprintf (format, args);
	va_end (args);
	printf("\n");
}
void lsllogdebug(const char* format, ...)
{
	va_list args;
	va_start (args, format);
	vprintf (format, args);
	va_end (args);
	printf("\n");
}
void lsllogwarning(const char* format, ...)
{
	va_list args;
	va_start (args, format);
	vprintf (format, args);
	va_end (args);
	printf("\n");
}


void dump(LSL::StringVector& vec)
{
	LSL::StringVector::iterator it;
	for (it = vec.begin(); it != vec.end(); ++it) {
		printf("%s\n", (*it).c_str());
	}
}

void GetMapInfo(LSL::StringVector& maps)
{
	for(const std::string mapname: maps) {
		lsllogdebug("Extracting %s", mapname.c_str());
		LSL::usync().GetMetalmap(mapname, 512, 512);
		LSL::usync().GetHeightmap(mapname, 512, 512);
		LSL::usync().GetMinimap(mapname, 512, 512);
		LSL::usync().GetMap(mapname);
	}
}

void GetGameInfo(LSL::StringVector& games)
{
}

void GetAIInfo()
{
}

int main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("Usage: %s <cache dir> <unitsync path>\n", argv[0]);
		return 1;
	}
	LSL::Util::config().ConfigurePaths(argv[1], argv[2], "");
	LSL::usync().LoadUnitSyncLib(argv[2]);

	LSL::StringVector maps = LSL::usync().GetMapList();
	GetMapInfo(maps);
	LSL::StringVector games = LSL::usync().GetGameList();
	GetGameInfo(games);
	//LSL::usync().
	LSL::usync().FreeUnitSyncLib();
}
