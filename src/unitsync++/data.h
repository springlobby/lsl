/**
	This file is part of SpringLobby,
	Copyright (C) 2007-2011

	SpringLobby is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License version 2 as published by
	the Free Software Foundation.

	SpringLobby is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with SpringLobby.  If not, see <http://www.gnu.org/licenses/>.
**/


#ifndef LSL_HEADERGUARD_SPRINGUNITSYNC_DATA_H
#define LSL_HEADERGUARD_SPRINGUNITSYNC_DATA_H

#include <vector>
#include <map>
#include <string>

namespace LSL {

struct UnitsyncMod
{
	UnitsyncMod() : name(std::string()),hash(std::string()) { }
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
};

struct UnitsyncMap
{
	UnitsyncMap() : name(std::string()),hash(std::string()) { }
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
	mod
};


typedef std::map<std::string,std::string> LocalArchivesVector;

} // namespace LSL
#endif // SPRINGLOBBY_HEADERGUARD_SPRINGUNITSYNC_DATA_H
