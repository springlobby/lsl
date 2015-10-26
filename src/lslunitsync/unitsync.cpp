/* This file is part of the Springlobby (GPL v2 or later), see COPYING */

#include "unitsync.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <clocale>
#include <set>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <iterator>

#include "c_api.h"
#include "image.h"
#include "springbundle.h"

#include <lslutils/config.h>
#include <lslutils/debug.h>
#include <lslutils/conversion.h>
#include <lslutils/misc.h>
#include <lslutils/globalsmanager.h>
#include <lslutils/thread.h>

#define LOCK_UNITSYNC boost::mutex::scoped_lock lock_criticalsection(m_lock)

#define ASYNC_LOAD 0 //FIXME: repair/set to 1!
#if ASYNC_LOAD
#define TRY_LOCK(ret)                                               \
	boost::mutex::scoped_try_lock lock_criticalsection(m_lock); \
	if (!lock_criticalsection.owns_lock()) {                    \
		return ret;                                         \
	}
#else
#define TRY_LOCK(ret)
#endif

#define ASSERT_EXCEPTION(cond, msg)               \
	do {                                      \
		if (!(cond)) {                    \
			LSL_THROW(unitsync, msg); \
		}                                 \
	} while (0)

namespace LSL
{

Unitsync::Unitsync()
    : m_cache_thread(new WorkerThread)
    , m_map_image_cache(30, "m_map_image_cache")
    , // may take about 300k per image ( 512x512 24 bpp minimap )
    m_tiny_minimap_cache(200, "m_tiny_minimap_cache")
    , // takes at most 30k per image (   100x100 24 bpp minimap )
    m_mapinfo_cache(1000000, "m_mapinfo_cache")
    ,					// this one is just misused as thread safe std::map ...
    m_sides_cache(200, "m_sides_cache") // another misuse
{
}


enum ASYNC_EVENTS {
	ASYNC_MAP_IMAGE_EVT = 1,
	ASYNC_MAP_IMAGE_SCALED_EVT,
	ASYNC_MAP_EX_EVT,
	ASYNC_UNITSYNC_LOADED_EVT,
};

Unitsync::~Unitsync()
{
	ClearCache();
	delete m_cache_thread;
	m_cache_thread = NULL;
}

bool CompareStringNoCase(const std::string& first, const std::string& second)
{
	//this doesn't compile on osx
	/*
	static std::locale l("C");
	static boost::is_iless il(l);
	try {
		//this fails on certain names
		return il(first,second);
	}
	catch(...) {
*/
	return first < second;
	//	}
}

bool Unitsync::LoadUnitSyncLib(const std::string& unitsyncloc)
{
	LOCK_UNITSYNC;
	ClearCache();
	bool ret = _LoadUnitSyncLib(unitsyncloc);
	if (ret) {
		m_cache_path = LSL::Util::config().GetCachePath();
		PopulateArchiveList();
	}
	return ret;
}

void Unitsync::ClearCache()
{
	m_maps_list.clear();
	m_mods_list.clear();
	m_mod_array.clear();
	m_map_array.clear();
	m_unsorted_mod_array.clear();
	m_unsorted_map_array.clear();
	m_map_image_cache.Clear();
	m_mapinfo_cache.Clear();
	m_sides_cache.Clear();
	m_map_gameoptions.clear();
	m_game_gameoptions.clear();
}

void Unitsync::FetchUnitsyncErrors(const std::string& prefix)
{
	auto errors = susynclib().GetUnitsyncErrors();
	std::string pre = prefix;
	if (!prefix.empty()) {
		pre += " ";
	}
	for (const std::string error : errors) {
		LslWarning("Unitsync: %s%s", pre.c_str(), error.c_str());
	}
}

static std::string GetGameInfo(int index, const std::string keyname)
{
	const int count = susynclib().GetPrimaryModInfoCount(index);
	for (int i = 0; i < count; i++) {
		const std::string key = Util::SafeString(susynclib().GetInfoKey(i));
		if (key == keyname) {
			return Util::SafeString(susynclib().GetInfoValueString(i));
		}
	}
	return "";
}

void Unitsync::PopulateArchiveList()
{

	const int numMaps = susynclib().GetMapCount();
	for (int i = 0; i < numMaps; i++) {
		std::string name, archivename;
		unsigned int hash;
		try {
			const int count = susynclib().GetMapArchiveCount(i);
			if (count > 0) {
				archivename = susynclib().GetMapArchiveName(0);
			}
			name = susynclib().GetMapName(i);
			hash = susynclib().GetMapChecksum(i);
			//PrefetchMap( name ); // DEBUG
		} catch (...) {
			continue;
		}
		try {
			assert(!name.empty());
			m_maps_list[name] = LSL::Util::ToUIntString(hash);
			if (!archivename.empty())
				m_maps_archive_name[name] = archivename;
			m_map_array.push_back(name);
		} catch (...) {
			LslError("Found map with hash collision: %s hash: %d", name.c_str(), hash);
		}
		FetchUnitsyncErrors(name);
	}
	const int numMods = susynclib().GetPrimaryModCount();
	for (int i = 0; i < numMods; i++) {
		std::string name, archivename;
		unsigned int hash;
		try {
			const int count = susynclib().GetPrimaryModArchiveCount(i);
			if (count > 0) {
				archivename = susynclib().GetPrimaryModArchive(i);
			}
			name = GetGameInfo(i, "name");
			hash = susynclib().GetPrimaryModChecksumFromName(name);
		} catch (...) {
			continue;
		}
		try {
			assert(!name.empty());
			m_mods_list[name] = LSL::Util::ToUIntString(hash);
			if (!archivename.empty())
				m_mods_archive_name[name] = archivename;
			m_mod_array.push_back(name);
		} catch (...) {
			LslError("Found game with hash collision: %s hash: %s", name.c_str(), hash);
		}
		FetchUnitsyncErrors(name);
	}
	m_unsorted_mod_array = m_mod_array;
	m_unsorted_map_array = m_map_array;
	std::sort(m_map_array.begin(), m_map_array.end(), &CompareStringNoCase);
	std::sort(m_mod_array.begin(), m_mod_array.end(), &CompareStringNoCase);
}

bool Unitsync::_LoadUnitSyncLib(const std::string& unitsyncloc)
{
	try {
		susynclib().Load(unitsyncloc);
	} catch (...) {
		return false;
	}
	return true;
}


void Unitsync::FreeUnitSyncLib()
{
	LOCK_UNITSYNC;

	susynclib().Unload();
}


bool Unitsync::IsLoaded() const
{
	return susynclib().IsLoaded();
}


std::string Unitsync::GetSpringVersion() const
{

	std::string ret;
	try {
		ret = susynclib().GetSpringVersion();
	} catch (...) {
	}
	return ret;
}


StringVector Unitsync::GetGameList() const
{
	TRY_LOCK(StringVector())
	return m_mod_array;
}

bool Unitsync::GameExists(const std::string& gamename, const std::string& hash) const
{
	TRY_LOCK(false)
	LocalArchivesVector::const_iterator itor = m_mods_list.find(gamename);
	if (itor == m_mods_list.end())
		return false;
	if (hash.empty() || hash == "0")
		return true;
	return itor->second == hash;
}

UnitsyncGame Unitsync::GetGame(const std::string& gamename)
{
	UnitsyncGame m;
	TRY_LOCK(m);
	m.name = gamename;
	m.hash = m_mods_list[gamename];
	return m;
}


UnitsyncGame Unitsync::GetGame(int index)
{
	UnitsyncGame m;
	TRY_LOCK(m);
	m.name = m_mod_array[index];
	m.hash = m_mods_list[m.name];
	return m;
}

StringVector Unitsync::GetMapList() const
{
	TRY_LOCK(StringVector())
	return m_map_array;
}

StringVector Unitsync::GetGameValidMapList(const std::string& gamename) const
{
	StringVector ret;
	TRY_LOCK(ret)
	try {
		unsigned int mapcount = susynclib().GetValidMapCount(gamename);
		for (unsigned int i = 0; i < mapcount; i++)
			ret.push_back(susynclib().GetValidMapName(i));
	} catch (Exceptions::unitsync& e) {
	}
	return ret;
}

bool Unitsync::MapExists(const std::string& mapname, const std::string& hash) const
{
	assert(!mapname.empty());
	TRY_LOCK(false)
	LocalArchivesVector::const_iterator itor = m_maps_list.find(mapname);
	if (itor == m_maps_list.end())
		return false;
	if (hash.empty() || hash == "0")
		return true;
	return itor->second == hash;
}

UnitsyncMap Unitsync::GetMap(int index)
{
	UnitsyncMap m;
	TRY_LOCK(m)
	if (index < 0)
		return m;
	m.name = m_map_array[index];
	m.hash = m_maps_list[m.name];
	m.info = _GetMapInfoEx(m.name);
	return m;
}

void GetOptionEntry(const int i, GameOptions& ret)
{
	//all section values for options are converted to lower case
	//since usync returns the key of section type keys lower case
	//otherwise comapring would be a real hassle
	const std::string key = susynclib().GetOptionKey(i);
	const std::string name = susynclib().GetOptionName(i);
	const std::string section_str = boost::algorithm::to_lower_copy(susynclib().GetOptionSection(i));
	const std::string optiondesc = susynclib().GetOptionDesc(i);
	const int opttype = susynclib().GetOptionType(i);
	switch (opttype) {
		case Enum::opt_float: {
			ret.float_map[key] = mmOptionFloat(name, key, optiondesc, susynclib().GetOptionNumberDef(i),
							   susynclib().GetOptionNumberStep(i),
							   susynclib().GetOptionNumberMin(i), susynclib().GetOptionNumberMax(i),
							   section_str, susynclib().GetOptionStyle(i));
			break;
		}
		case Enum::opt_bool: {
			ret.bool_map[key] = mmOptionBool(name, key, optiondesc, susynclib().GetOptionBoolDef(i), section_str, susynclib().GetOptionStyle(i));
			break;
		}
		case Enum::opt_string: {
			ret.string_map[key] = mmOptionString(name, key, optiondesc, susynclib().GetOptionStringDef(i), susynclib().GetOptionStringMaxLen(i), section_str, susynclib().GetOptionStyle(i));
			break;
		}
		case Enum::opt_list: {
			ret.list_map[key] = mmOptionList(name, key, optiondesc, susynclib().GetOptionListDef(i), section_str, susynclib().GetOptionStyle(i));
			const int listItemCount = susynclib().GetOptionListCount(i);
			for (int j = 0; j < listItemCount; ++j) {
				std::string descr = susynclib().GetOptionListItemDesc(i, j);
				ret.list_map[key].addItem(susynclib().GetOptionListItemKey(i, j), susynclib().GetOptionListItemName(i, j), descr);
			}
			break;
		}
		case Enum::opt_section: {
			ret.section_map[key] = mmOptionSection(name, key, optiondesc, section_str, susynclib().GetOptionStyle(i));
		}
	}
}


GameOptions Unitsync::GetMapOptions(const std::string& name)
{
	GameOptions ret;
	TRY_LOCK(ret)

	assert(!name.empty());
	if (m_map_gameoptions.find(name) != m_map_gameoptions.end()) {
		return m_map_gameoptions[name];
	}

	int count = susynclib().GetMapOptionCount(name);
	for (int i = 0; i < count; ++i) {
		GetOptionEntry(i, ret);
	}
	m_map_gameoptions[name] = ret;
	return ret;
}

StringVector Unitsync::GetMapDeps(const std::string& mapname)
{
	assert(!mapname.empty());
	StringVector ret;
	try {
		ret = susynclib().GetMapDeps(Util::IndexInSequence(m_unsorted_map_array, mapname));
	} catch (Exceptions::unitsync& u) {
	}
	return ret;
}

UnitsyncMap Unitsync::GetMap(const std::string& mapname)
{
	assert(!mapname.empty());
	const int i = Util::IndexInSequence(m_map_array, mapname);
	UnitsyncMap m;
	if (i < 0) {
		LSL_THROWF(unitsync, "Map does not exist: %s", mapname.c_str());
	}
	m.name = m_map_array[i];
	m.hash = m_maps_list[m.name];
	m.info = _GetMapInfoEx(m.name);
	return m;
}

GameOptions Unitsync::GetGameOptions(const std::string& name)
{
	assert(!name.empty());
	GameOptions ret;
	TRY_LOCK(ret)
	if (m_game_gameoptions.find(name) != m_game_gameoptions.end()) {
		return m_game_gameoptions[name];
	}
	if (!IsLoaded())
		return ret;
	int count = susynclib().GetModOptionCount(name);
	for (int i = 0; i < count; ++i) {
		GetOptionEntry(i, ret);
	}
	m_game_gameoptions[name] = ret;
	return ret;
}

StringVector Unitsync::GetGameDeps(const std::string& gamename) const
{
	assert(!gamename.empty());
	StringVector ret;
	TRY_LOCK(ret)
	try {
		ret = susynclib().GetModDeps(Util::IndexInSequence(m_unsorted_mod_array, gamename));
	} catch (Exceptions::unitsync& u) {
	}
	return ret;
}

StringVector Unitsync::GetSides(const std::string& gamename)
{
	assert(!gamename.empty());
	StringVector ret;
	TRY_LOCK(ret);
	const std::string cachefile = GetFileCachePath(gamename, true) + ".sides";
	if (m_sides_cache.TryGet(cachefile, ret)) { //first return from mru cache
		return ret;
	}

	if (!GetCacheFile(cachefile, ret) && (GameExists(gamename))) { // cache file failed, try from lsl
		try {
			ret = susynclib().GetSides(gamename);
			SetCacheFile(cachefile, ret); //store into cachefile
		} catch (Exceptions::unitsync& u) {
			LslWarning("Error in GetSides: %s %s", gamename.c_str(), u.what());
			return ret;
		}
	}
	m_sides_cache.Add(cachefile, ret); //store into mru
	return ret;
}


UnitsyncImage Unitsync::GetSidePicture(const std::string& gamename, const std::string& SideName)
{
	assert(!gamename.empty());

	const std::string cachepath = GetFileCachePath(gamename, true, false) + "-side-" + SideName + ".png";
	UnitsyncImage img;
	TRY_LOCK(img);

	if (Util::FileExists(cachepath)) {
		img = UnitsyncImage(cachepath);
	}

	if (!img.isValid()) { //image seems invalid, recreate
		std::string ImgName("SidePics");
		ImgName += "/";
		ImgName += boost::to_lower_copy(SideName);
		try {
			img = GetImage(gamename, ImgName + ".png", false);
		} catch (Exceptions::unitsync& u) {
		}
		if (!img.isValid()) {
			try {
				img = GetImage(gamename, ImgName + ".bmp", true);
			} catch (Exceptions::unitsync& u) {
			}
		}

		if (img.isValid()) {
			img.Save(cachepath);
		}
	}
	return img;
}

UnitsyncImage Unitsync::GetImage(const std::string& gamename, const std::string& image_path, bool useWhiteAsTransparent) const
{
	assert(!gamename.empty());
	susynclib().SetCurrentMod(gamename);
	int ini = susynclib().OpenFileVFS(image_path);
	if (!ini)
		LSL_THROWF(unitsync, "%s: cannot find image %s\n", gamename.c_str(), image_path.c_str());
	int FileSize = susynclib().FileSizeVFS(ini);
	if (FileSize == 0) {
		susynclib().CloseFileVFS(ini);
		LSL_THROWF(unitsync, "%s: image has size 0 %s\n", gamename.c_str(), image_path.c_str());
	}
	Util::uninitialized_array<char> FileContent(FileSize);
	susynclib().ReadFileVFS(ini, FileContent, FileSize);
	return UnitsyncImage::FromVfsFileData(FileContent, FileSize, image_path, useWhiteAsTransparent);
}

StringVector Unitsync::GetAIList(const std::string& gamename) const
{
	StringVector ret;
	TRY_LOCK(ret);
	if (gamename.empty())
		return ret;
	int total = susynclib().GetSkirmishAICount(gamename);
	for (int i = 0; i < total; i++) {
		StringVector infos = susynclib().GetAIInfo(i);
		const int namepos = Util::IndexInSequence(infos, "shortName");
		const int versionpos = Util::IndexInSequence(infos, "version");
		std::string ainame;
		if (namepos != lslNotFound)
			ainame += infos[namepos + 1];
		if (versionpos != lslNotFound)
			ainame += " " + infos[versionpos + 1];
		ret.push_back(ainame);
	}
	return ret;
}

void Unitsync::UnSetCurrentArchive()
{
#if ASYNC_LOAD
	LOCK_UNITSYNC;
#endif
	try {
		susynclib().UnSetCurrentMod();
	} catch (std::runtime_error) {
	}
}

StringVector Unitsync::GetAIInfos(int index) const
{
	StringVector ret;
	TRY_LOCK(ret);
	try {
		ret = susynclib().GetAIInfo(index);
	} catch (std::runtime_error) {
	}
	return ret;
}

GameOptions Unitsync::GetAIOptions(const std::string& gamename, int index)
{
	assert(!gamename.empty());
	GameOptions ret;
	TRY_LOCK(ret);
	int count = susynclib().GetAIOptionCount(gamename, index);
	for (int i = 0; i < count; ++i) {
		GetOptionEntry(i, ret);
	}
	return ret;
}

StringVector Unitsync::GetUnitsList(const std::string& gamename)
{
	assert(!gamename.empty());
	const std::string cachefile = GetFileCachePath(gamename, true) + ".units";
	StringVector cache;
	TRY_LOCK(cache)

	if (!GetCacheFile(cachefile, cache)) { //cache read failed
		susynclib().SetCurrentMod(gamename);
		while (susynclib().ProcessUnitsNoChecksum() > 0) {
		}
		const int unitcount = susynclib().GetUnitCount();
		for (int i = 0; i < unitcount; i++) {
			cache.push_back(susynclib().GetFullUnitName(i) + " (" + susynclib().GetUnitName(i) + ")");
		}
		SetCacheFile(cachefile, cache);
	}
	return cache;
}

UnitsyncImage Unitsync::GetMinimap(const std::string& mapname, int width, int height)
{
	UnitsyncImage img;
	TRY_LOCK(img)
	if (mapname.empty()) {
		return img;
	}
	return GetScaledMapImage(mapname, IMAGE_MAP, width, height);
}

UnitsyncImage Unitsync::GetMetalmap(const std::string& mapname, int width, int height)
{
	TRY_LOCK(UnitsyncImage())
	return GetScaledMapImage(mapname, IMAGE_METALMAP, width, height);
}


UnitsyncImage Unitsync::GetHeightmap(const std::string& mapname, int width, int height)
{
	TRY_LOCK(UnitsyncImage())
	return GetScaledMapImage(mapname, IMAGE_HEIGHTMAP, width, height);
}


UnitsyncImage Unitsync::GetScaledMapImage(const std::string& mapname, ImageType imgtype, int width, int height)
{
	UnitsyncImage img;
    std::string imagename;
	switch(imgtype) {
		case IMAGE_MAP:
            imagename = ".minimap.png";
			break;
		case IMAGE_METALMAP:
			imagename = ".metalmap.png";
			break;
		case IMAGE_HEIGHTMAP:
            imagename = ".heightmap.png";
			break;
	}
	assert(!imagename.empty());
	const bool rescale = (width > 0) && (height > 0);
	const bool tiny = (width <= 100 && height <= 100);
	bool loaded = false;
	if (tiny && m_tiny_minimap_cache.TryGet(mapname, img)) {
		loaded = img.isValid();
	}
	if (!loaded && m_map_image_cache.TryGet(mapname + imagename, img)) {
		loaded = img.isValid();
	}

	const std::string cachefile = GetFileCachePath(mapname, false, false) + imagename;
	if (!loaded) {
		if (Util::FileExists(cachefile)) {
			img = UnitsyncImage(cachefile);
		}
		loaded = img.isValid();
	}

	if (!loaded) { //image seems invalid, recreate
		try {
			//convert and save
			switch(imgtype) {
				case IMAGE_MAP:
					img = susynclib().GetMinimap(mapname);
					break;
				case IMAGE_METALMAP:
					img = susynclib().GetMetalmap(mapname);
					break;
				case IMAGE_HEIGHTMAP:
					img = susynclib().GetHeightmap(mapname);
					break;
			}
			img.Save(cachefile);
		} catch (...) { //we failed horrible, use dummy image
			//dummy image
			img = UnitsyncImage(1, 1);
		}
	}

	m_map_image_cache.Add(mapname + imagename, img); //cache before rescale

	if (rescale && img.isValid()) {
		lslSize image_size = lslSize(img.GetWidth(), img.GetHeight()).MakeFit(lslSize(width, height));
		if (rescale && (image_size.GetWidth() != img.GetWidth() || image_size.GetHeight() != img.GetHeight())) {
			img.Rescale(image_size.GetWidth(), image_size.GetHeight());
		}
	}

	if (tiny) {
			m_tiny_minimap_cache.Add(mapname + imagename, img);
	}
	return img;
}

MapInfo Unitsync::_GetMapInfoEx(const std::string& mapname)
{
	MapInfo info;
	info.width = 1;
	info.height = 1;
	if (m_mapinfo_cache.TryGet(mapname, info))
		return info;
	const std::string cachefile = GetFileCachePath(mapname, false, false) + ".mapinfo";
	StringVector cache;
	if (GetCacheFile(cachefile, cache) && cache.size() >= 11) { //cache file failed
		info.author = cache[0];
		info.tidalStrength = Util::FromFloatString(cache[1]);
		info.gravity = Util::FromIntString(cache[2]);
		info.maxMetal = Util::FromFloatString(cache[3]);
		info.extractorRadius = Util::FromFloatString(cache[4]);
		info.minWind = Util::FromIntString(cache[5]);
		info.maxWind = Util::FromIntString(cache[6]);
		info.width = Util::FromIntString(cache[7]);
		info.height = Util::FromIntString(cache[8]);
		const StringVector posinfo = Util::StringTokenize(cache[9], " ");
		for (const std::string pos : posinfo) {
			StartPos position;
			position.x = Util::FromIntString(Util::BeforeFirst(pos, "-"));
			position.y = Util::FromIntString(Util::AfterFirst(pos, "-"));
			info.positions.push_back(position);
		}
		const unsigned int LineCount = cache.size();
		for (unsigned int i = 10; i < LineCount; i++)
			info.description += cache[i] + "\n";
	} else {
		const int index = Util::IndexInSequence(m_unsorted_map_array, mapname);
		ASSERT_EXCEPTION(index >= 0, "Map not found");

		info = susynclib().GetMapInfoEx(index, 1);

		cache.push_back(info.author);
		cache.push_back(Util::ToFloatString(info.tidalStrength));
		cache.push_back(Util::ToIntString(info.gravity));
		cache.push_back(Util::ToFloatString(info.maxMetal));
		cache.push_back(Util::ToFloatString(info.extractorRadius));
		cache.push_back(Util::ToFloatString(info.minWind));
		cache.push_back(Util::ToFloatString(info.maxWind));
		cache.push_back(Util::ToIntString(info.width));
		cache.push_back(Util::ToIntString(info.height));

		std::string postring;
		for (unsigned int i = 0; i < info.positions.size(); i++) {
			if (!postring.empty()) {
				postring += " ";
			}
			postring += Util::ToIntString(info.positions[i].x) + "-" + Util::ToIntString(info.positions[i].y);
		}
		cache.push_back(postring);

		const StringVector descrtokens = Util::StringTokenize(info.description, "\n");
		for (const std::string descrtoken : descrtokens) {
			cache.push_back(descrtoken);
		}
		SetCacheFile(cachefile, cache);
	}

	m_mapinfo_cache.Add(mapname, info);

	return info;
}

bool Unitsync::ReloadUnitSyncLib()
{
#if ASYNC_LOAD
	LOCK_UNITSYNC;
#endif
	const std::string path = LSL::Util::config().GetCurrentUsedUnitSync();
	if (path.empty())
		return false;
#if ASYNC_LOAD
	LoadUnitSyncLibAsync(path);
#else
	LoadUnitSyncLib(path);
#endif
	return true;
}


void Unitsync::SetSpringDataPath(const std::string& path)
{
	if (!IsLoaded()) {
		return;
	}
#if ASYNC_LOAD
	LOCK_UNITSYNC;
#endif

	susynclib().SetSpringConfigString("SpringData", path);
}

bool Unitsync::GetSpringDataPath(std::string& path)
{
	TRY_LOCK(false)
	if (IsLoaded()) {
		path = susynclib().GetSpringDataDir();
	}
	return !path.empty();
}

std::string Unitsync::GetFileCachePath(const std::string& name, bool IsMod, bool usehash)
{
	assert(!name.empty());
	std::string ret = m_cache_path + name;
	if (!usehash)
		return ret;

	if (IsMod) {
		ret += "-" + m_mods_list[name];
	} else {
		ret += "-" + m_maps_list[name];
	}
	return ret;
}

bool Unitsync::GetCacheFile(const std::string& path, StringVector& ret) const
{
	FILE* file = Util::lslopen(path, "r");
	if (file == NULL)
		return false;
	ret.clear();
	char line[1024];
	while (fgets(line, 1024, file) != NULL) {
		const int len = strnlen(line, 1024);
		ret.push_back(std::string(line, len - 1));
	}
	fclose(file);
	return true;
}

void Unitsync::SetCacheFile(const std::string& path, const StringVector& data)
{
	FILE* file = Util::lslopen(path, "w");

	ASSERT_EXCEPTION(file != NULL, (boost::format("cache file( %s ) not found") % path).str().c_str());

	for (std::string line : data) {
		line += "\n";
		fwrite(line.c_str(), line.size(), 1, file);
	}
	fclose(file);
}

StringVector Unitsync::GetPlaybackList(bool ReplayType) const
{
	StringVector ret;
	TRY_LOCK(ret)
	if (!IsLoaded())
		return ret;
	std::string type;
	std::string subpath;
	if (ReplayType) {
		type = ".sdf";
		subpath = "demos";
	} else {
		type = ".ssf";
		subpath = "Saves";
	}
	const int count = susynclib().GetSpringDataDirCount();
	StringVector paths;
	for (int i = 0; i < count; i++) {
		const std::string datadir = susynclib().GetSpringDataDirByIndex(i);
		if (datadir.empty()) {
			continue;
		}
		paths.push_back(datadir);
	}
	try {
		for (const std::string datadir : paths) {
			const std::string dir = Util::EnsureDelimiter(datadir) + subpath;
			try {
				if (!boost::filesystem::is_directory(dir)) {
					continue;
				}
			} catch (...) {
			}
			boost::filesystem::directory_iterator enditer;
			for (boost::filesystem::directory_iterator dir_iter(dir); dir_iter != enditer; ++dir_iter) {
				if (!boost::filesystem::is_regular_file(dir_iter->status()))
					continue;
				const std::string filename(dir_iter->path().string());
				if (filename.substr(filename.length() - 4) != type) // compare file ending
					continue;
				ret.push_back(filename);
			}
		}
	} catch (...) {
	}
	return ret;
}

bool Unitsync::FileExists(const std::string& name) const
{
	assert(!name.empty());
	assert(name[0] != '/'); // don't check for real files here, as this is a VERY slow call
	int handle = susynclib().OpenFileVFS(name);
	if (handle == 0)
		return false;
	susynclib().CloseFileVFS(handle);
	return true;
}

std::string Unitsync::GetArchivePath(const std::string& name) const
{
#if ASYNC_LOAD
	LOCK_UNITSYNC;
#endif
	return susynclib().GetArchivePath(name);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// Unitsync prefetch/background thread code

namespace
{
class CacheMapImageWorkItem : public WorkItem
{
public:
	Unitsync* m_usync;
	std::string m_mapname;

	void Run()
	{
		m_usync->GetScaledMapImage(m_mapname, m_imgtype, m_width, m_height);
		m_usync->PostEvent(m_mapname);
	}
	CacheMapImageWorkItem(Unitsync* usync, const std::string& mapname, LSL::ImageType imgtype, int w = -1, int h = -1)
	    : m_usync(usync)
	    , m_mapname(mapname)
	    , m_width(w)
	    , m_height(h)
	    , m_imgtype(imgtype)
	{
		assert(usync != nullptr);
	}
private:
	int m_width;
	int m_height;
	LSL::ImageType m_imgtype;
	CacheMapImageWorkItem(){}
};

class GetMapExAsyncWorkItem : public WorkItem
{
public:
	void Run()
	{
		m_usync->GetMap(m_mapname);
		m_usync->PostEvent(m_mapname);
	}

	GetMapExAsyncWorkItem(Unitsync* usync, const std::string& mapname):
	    m_usync(usync),
		m_mapname(mapname)
	{
	}
private:
	Unitsync* m_usync;
	std::string m_mapname;
};

class LoadUnitSyncLibAsyncWorkItem : public WorkItem
{
public:
	void Run()
	{
		try {
			m_usync->LoadUnitSyncLib(m_unitsyncloc);
		} catch (...) {
			// Event without mapname means some async job failed.
			// This is sufficient for now, we just need symmetry between
			// number of initiated async jobs and number of finished/failed
			// async jobs.
		}
		m_usync->PostEvent("");
	}

private:
	Unitsync* m_usync;
	std::string m_unitsyncloc;

public:
	LoadUnitSyncLibAsyncWorkItem(Unitsync* usync, const std::string& unitsyncLoc)
	    : m_usync(usync)
	    , m_unitsyncloc(unitsyncLoc.c_str())
	{
	}
};
}


void Unitsync::PrefetchMap(const std::string& mapname)
{
	// Use a simple hash based on 3 characters from the mapname
	// (without '.smf') as negative priority for the WorkItems.
	// This ensures WorkItems for the same map are put together,
	// which improves caching performance.

	// Measured improvement: 60% more cache hits while populating replay tab.
	// 50% hits without, 80% hits with this code.  (cache size 20 images)
	assert(!mapname.empty());

//	const int length = std::max(0, int(mapname.length()) - 4);
//	const int hash = (mapname[length * 1 / 4] << 16) | (mapname[length * 2 / 4] << 8) | mapname[length * 3 / 4];
//	const int priority = -hash;

	if (!m_cache_thread) {
		LslDebug("cache thread not initialized %s", "PrefetchMap");
		return;
	}
	GetMap(mapname);
	GetScaledMapImage(mapname, IMAGE_MAP);
	GetScaledMapImage(mapname, IMAGE_METALMAP);
	GetScaledMapImage(mapname, IMAGE_HEIGHTMAP);
}

boost::signals2::connection Unitsync::RegisterEvtHandler(const StringSignalSlotType& handler)
{
	return m_async_ops_complete_sig.connect(handler);
}

void Unitsync::UnregisterEvtHandler(boost::signals2::connection& conn)
{
	conn.disconnect();
}

void Unitsync::PostEvent(const std::string& evt)
{
	m_async_ops_complete_sig(evt);
}

void Unitsync::GetMinimapAsync(const std::string& mapname, int width, int height)
{
	if (mapname.empty())
		return;
	if (!m_cache_thread) {
		LslError("cache thread not initialised");
		return;
	}
	CacheMapImageWorkItem* work = new CacheMapImageWorkItem(this, mapname, IMAGE_MAP, width, height);
	m_cache_thread->DoWork(work, 100);
}


void Unitsync::GetMetalmapAsync(const std::string& mapname, int width, int height)
{
	assert(!mapname.empty());
	CacheMapImageWorkItem* work = new CacheMapImageWorkItem(this, mapname, IMAGE_METALMAP, width, height);
	m_cache_thread->DoWork(work, 100);
}

void Unitsync::GetHeightmapAsync(const std::string& mapname, int width, int height)
{
	assert(!mapname.empty());
	CacheMapImageWorkItem* work = new CacheMapImageWorkItem(this, mapname, IMAGE_HEIGHTMAP, width, height);
	m_cache_thread->DoWork(work, 100);
}

void Unitsync::GetMapExAsync(const std::string& mapname)
{
	assert(!mapname.empty());
	if (mapname.empty())
		return;

	if (!m_cache_thread) {
		LslDebug("cache thread not initialized %s", "GetMapExAsync");
		return;
	}
	GetMapExAsyncWorkItem* work;
	work = new GetMapExAsyncWorkItem(this, mapname);
	m_cache_thread->DoWork(work, 200 /* higher prio then GetMinimapAsync */);
}

std::string Unitsync::GetTextfileAsString(const std::string& gamename, const std::string& file_path)
{
	assert(!gamename.empty());
	susynclib().SetCurrentMod(gamename);

	int ini = susynclib().OpenFileVFS(file_path);
	if (!ini)
		return std::string();
	int FileSize = susynclib().FileSizeVFS(ini);
	if (FileSize == 0) {
		susynclib().CloseFileVFS(ini);
		return std::string();
	}
	Util::uninitialized_array<char> FileContent(FileSize);
	susynclib().ReadFileVFS(ini, FileContent, FileSize);
	return std::string(FileContent, size_t(FileSize));
}

Unitsync& usync()
{
	static LSL::Util::LineInfo<Unitsync> m(AT);
	static LSL::Util::GlobalObjectHolder<Unitsync, LSL::Util::LineInfo<Unitsync> > m_sync(m);
	return m_sync;
}

void Unitsync::LoadUnitSyncLibAsync(const std::string& filename)
{
	LoadUnitSyncLibAsyncWorkItem* work = new LoadUnitSyncLibAsyncWorkItem(this, filename);
	m_cache_thread->DoWork(work, 500);
}

int Unitsync::GetSpringConfigInt(const std::string& name, int defvalue)
{
	if (IsLoaded())
		return susynclib().GetSpringConfigInt(name, defvalue);
	return defvalue;
}

std::string Unitsync::GetSpringConfigString(const std::string& name, const std::string& defvalue)
{
	if (IsLoaded())
		return susynclib().GetSpringConfigString(name, defvalue);
	return defvalue;
}

float Unitsync::GetSpringConfigFloat(const std::string& name, float defvalue)
{
	if (IsLoaded())
		return susynclib().GetSpringConfigInt(name, defvalue);
	return defvalue;
}

void Unitsync::SetSpringConfigInt(const std::string& name, int value)
{
	return susynclib().SetSpringConfigInt(name, value);
}

void Unitsync::SetSpringConfigString(const std::string& name, const std::string& value)
{
	return susynclib().SetSpringConfigString(name, value);
}

void Unitsync::SetSpringConfigFloat(const std::string& name, float value)
{
	return susynclib().SetSpringConfigFloat(name, value);
}

std::string Unitsync::GetConfigFilePath()
{
	return susynclib().GetConfigFilePath();
}

} // namespace LSL
