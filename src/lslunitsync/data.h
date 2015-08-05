#ifndef LSL_HEADERGUARD_SPRINGUNITSYNC_DATA_H
#define LSL_HEADERGUARD_SPRINGUNITSYNC_DATA_H

#include <vector>
#include <map>
#include <string>

namespace LSL {

struct UnitsyncGame
{
    UnitsyncGame()
        : name(std::string()),hash(std::string())
    {}
    UnitsyncGame(const std::string& name, const std::string& hash)
        : name(name),hash(hash)
    {}
	std::string name;
	std::string hash;
};

struct StartPos
{
	int x;
	int y;
};

struct MapInfo
{
	std::string description;
	int tidalStrength;
	int gravity;
	float maxMetal;
	int extractorRadius;
	int minWind;
	int maxWind;

	int width;
	int height;
	std::vector<StartPos> positions;

	std::string author;
	MapInfo():
		description(""),
		tidalStrength(0),
		gravity(0),
		maxMetal(0.0f),
		extractorRadius(0),
		minWind(0),
		maxWind(0),
		width(0),
		height(0),
		author("")
	{
	}
};

struct UnitsyncMap
{
    UnitsyncMap():
		name(std::string()),
        hash(std::string())
    {}
    UnitsyncMap(const std::string& name, const std::string& hash):
		name(name),
		hash(hash)
    {}
	std::string name;
	std::string hash;
	MapInfo info;
};

enum GameFeature
{
	USYNC_Sett_Handler,
	USYNC_GetInfoMap,
	USYNC_GetDataDir,
	USYNC_GetSkirmishAI
};

enum MediaType
{
	map,
	game
};


typedef std::map<std::string,std::string> LocalArchivesVector;

} // namespace LSL

/**
 * \file data.h
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

#endif // SPRINGLOBBY_HEADERGUARD_SPRINGUNITSYNC_DATA_H
