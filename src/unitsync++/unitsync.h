#ifndef SPRINGLOBBY_HEADERGUARD_SPRINGUNITSYNC_H
#define SPRINGLOBBY_HEADERGUARD_SPRINGUNITSYNC_H

#include "thread.h"
#include "mmoptionmodel.h"
#include "data.h"
#include "mru_cache.h"

#include <boost/thread/mutex.hpp>
#include <map>

namespace LSL {

class UnitsyncImage;
extern const wxEventType UnitSyncAsyncOperationCompletedEvt;
struct GameOptions;
struct CachedMapInfo;
struct SpringMapInfo;
class SpringUnitSyncLib;

/// Thread safe mapping from evtHandlerId to wxEvtHandler*
class EvtHandlerCollection
{
  public:
    EvtHandlerCollection() : m_last_id(0) {}

    int Add( wxEvtHandler* evtHandler );
    void Remove( int evtHandlerId );
    void PostEvent( int evtHandlerId, wxEvent& evt );

  private:
    typedef std::map<int, wxEvtHandler*> EvtHandlerMap;

	boost::mutex m_lock;
    EvtHandlerMap m_items;
    int m_last_id;
};

class SpringUnitSync
{
private:
	SpringUnitSync();

	typedef std::vector< std::string >
		StringVector;
public:
	virtual ~SpringUnitSync();

	typedef std::map<std::string,mmOptionBool> OptionMapBool;
	typedef std::map<std::string,mmOptionFloat> OptionMapFloat;
	typedef std::map<std::string,mmOptionString> OptionMapString;
	typedef std::map<std::string,mmOptionList> OptionMapList;
	typedef std::map<std::string,mmOptionSection> OptionMapSection;

	typedef std::map<std::string,mmOptionBool>::iterator OptionMapBoolIter;
	typedef std::map<std::string,mmOptionFloat>::iterator OptionMapFloatIter;
	typedef std::map<std::string,mmOptionString>::iterator OptionMapStringIter;
	typedef std::map<std::string,mmOptionList>::iterator OptionMapListIter;
	typedef std::map<std::string,mmOptionSection>::iterator OptionMapSectionIter;

	typedef std::map<std::string,mmOptionBool>::const_iterator OptionMapBoolConstIter;
	typedef std::map<std::string,mmOptionFloat>::const_iterator OptionMapFloatConstIter;
	typedef std::map<std::string,mmOptionString>::const_iterator OptionMapStringConstIter;
	typedef std::map<std::string,mmOptionList>::const_iterator OptionMapListConstIter;
	typedef std::map<std::string,mmOptionSection>::const_iterator OptionMapSectionConstIter;

	int GetNumMods() const;
    StringVector GetModList() const;
	bool ModExists( const std::string& modname ) const;
	bool ModExists( const std::string& modname, const std::string& hash ) const;
    bool ModExistsCheckHash( const std::string& hash ) const;
	UnitSyncMod GetMod( const std::string& modname );
	UnitSyncMod GetMod( int index );
	//! this functions returns index CUSTOM ALPHBETICALLY SORTED, DO NOT USE TO ACCESS UNITSYNC DIRECTLY
	//! use m_unsorted_mod_array for real unitsync index
	int GetModIndex( const std::string& name ) const;
    GameOptions GetModOptions( const std::string& name );
    StringVector GetModDeps( const std::string& name ) const;

	int GetNumMaps() const;
    StringVector GetMapList() const;
    StringVector GetModValidMapList( const std::string& modname ) const;
	bool MapExists( const std::string& mapname ) const;
	bool MapExists( const std::string& mapname, const std::string& hash ) const;

    UnitSyncMap GetMap( const std::string& mapname );
	UnitSyncMap GetMap( int index );
    UnitSyncMap GetMapEx( const std::string& mapname );
	UnitSyncMap GetMapEx( int index );
    GameOptions GetMapOptions( const std::string& name );
    StringVector GetMapDeps( const std::string& name );

    //! function to fetch default singplayer/replay/savegame's default nick
	std::string GetDefaultNick();
	//! function to set default singplayer/replay/savegame's default nick
	void SetDefaultNick( const std::string& nick );
	//! this functions returns index CUSTOM ALPHBETICALLY SORTED, DO NOT USE TO ACCESS UNITSYNC DIRECTLY
	//! use m_unsorted_map_array for real unitsync index
	int GetMapIndex( const std::string& name ) const;

    StringVector GetSides( const std::string& modname  );
	UnitsyncImage GetSidePicture( const std::string& modname, const std::string& SideName ) const;
	UnitsyncImage GetImage( const std::string& modname, const std::string& image_path, bool useWhiteAsTransparent = true ) const;

    bool LoadUnitSyncLib( const std::string& unitsyncloc );
    void FreeUnitSyncLib();

	bool IsLoaded() const;

	std::string GetSpringVersion() const;
    //! function wich checks if the version returned from unitsync matches a table of supported feature
	bool VersionSupports( GameFeature feature ) const;

	void UnSetCurrentMod();

    StringVector GetAIList( const std::string& modname ) const;
    StringVector GetAIInfos( int index ) const;
    GameOptions GetAIOptions( const std::string& modname, int index );


	int GetNumUnits( const std::string& modname ) const;
    StringVector GetUnitsList( const std::string& modname );

    /// get minimap with native width x height
    UnitsyncImage GetMinimap( const std::string& mapname );
    /// get minimap rescaled to given width x height
    UnitsyncImage GetMinimap( const std::string& mapname, int width, int height );
    /// get metalmap with native width x height
    UnitsyncImage GetMetalmap( const std::string& mapname );
    /// get metalmap rescaled to given width x height
    UnitsyncImage GetMetalmap( const std::string& mapname, int width, int height );
    /// get heightmap with native width x height
    UnitsyncImage GetHeightmap( const std::string& mapname );
    /// get heightmap rescaled to given width x height
    UnitsyncImage GetHeightmap( const std::string& mapname, int width, int height );

	std::string GetTextfileAsString( const std::string& modname, const std::string& file_path );

	bool ReloadUnitSyncLib(  );
//	void ReloadUnitSyncLib( GlobalEvents::GlobalEventData /*data*/ ) { ReloadUnitSyncLib(); }
	bool FastLoadUnitSyncLib( const std::string& unitsyncloc );
	bool FastLoadUnitSyncLibInit();

    void SetSpringDataPath( const std::string& path );

    StringVector GetPlaybackList( bool ReplayType = true ) const; //savegames otehrwise

	bool FileExists( const std::string& name ) const;

	std::string GetArchivePath( const std::string& name ) const;

    /// schedule a map for prefetching
    void PrefetchMap( const std::string& mapname );

    int RegisterEvtHandler( wxEvtHandler* evtHandler );
    void UnregisterEvtHandler( int evtHandlerId );
    void PostEvent( int evtHandlerId, wxEvent& evt ); // helper for WorkItems

    void GetMinimapAsync( const std::string& mapname, int evtHandlerId );
    void GetMinimapAsync( const std::string& mapname, int width, int height, int evtHandlerId );
    void GetMetalmapAsync( const std::string& mapname, int evtHandlerId );
    void GetMetalmapAsync( const std::string& mapname, int width, int height, int evtHandlerId );
    void GetHeightmapAsync( const std::string& mapname, int evtHandlerId );
    void GetHeightmapAsync( const std::string& mapname, int width, int height, int evtHandlerId );
    void GetMapExAsync( const std::string& mapname, int evtHandlerId );

    StringVector GetScreenshotFilenames() const;

    virtual GameOptions GetModCustomizations( const std::string& modname );
    virtual GameOptions GetSkirmishOptions( const std::string& modname, const std::string& skirmish_name );

	virtual void OnReload( wxCommandEvent& event );
	virtual void AddReloadEvent(  );

    StringVector FindFilesVFS( const std::string& pattern ) const;

  private:
	typedef std::map< std::pair<std::string,std::string>, std::string> ShortnameVersionToNameMap;
	ShortnameVersionToNameMap m_shortname_to_name_map;

    LocalArchivesVector m_maps_list; /// mapname -> hash
    LocalArchivesVector m_mods_list; /// modname -> hash
    LocalArchivesVector m_mods_unchained_hash; /// modname -> unchained hash
    LocalArchivesVector m_maps_unchained_hash; /// mapname -> unchained hash
    LocalArchivesVector m_mods_archive_name; /// modname -> archive name
    LocalArchivesVector m_maps_archive_name; /// mapname -> archive name
    StringVector m_map_array; // this vector is CUSTOM SORTED ALPHABETICALLY, DON'T USE TO ACCESS UNITSYNC DIRECTLY
    StringVector m_mod_array; // this vector is CUSTOM SORTED ALPHABETICALLY, DON'T USE TO ACCESS UNITSYNC DIRECTLY
    StringVector m_unsorted_map_array; // this is because unitsync doesn't have a search map index by name ..
    StringVector m_unsorted_mod_array; // this isn't necessary but makes things more symmetrical :P

    /// caches sett().GetCachePath(), because that method calls back into
    /// susynclib(), there's a good chance main thread blocks on some
    /// WorkerThread operation... cache is invalidated on reload.
    std::string m_cache_path;

	mutable boost::mutex m_lock;
	WorkerThread* m_cache_thread;
    EvtHandlerCollection m_evt_handlers;

    /// this cache facilitates async image fetching (image is stored in cache
    /// in background thread, then main thread gets it from cache)
    MostRecentlyUsedImageCache m_map_image_cache;
    /// this cache is a real cache, it stores minimaps with max size 100x100
    MostRecentlyUsedImageCache m_tiny_minimap_cache;

    /// this caches MapInfo to facilitate GetMapExAsync
    MostRecentlyUsedMapInfoCache m_mapinfo_cache;

    MostRecentlyUsedArrayStringCache m_sides_cache;

    //! this function returns only the cache path without the file extension,
    //! the extension itself would be added in the function as needed
    std::string GetFileCachePath( const std::string& name, const std::string& hash, bool IsMod );

    //! returns an array where each element is a line of the file
    StringVector GetCacheFile( const std::string& path ) const;
    //! write a file where each element of the array is a line
    void SetCacheFile( const std::string& path, const StringVector& data );

    bool _LoadUnitSyncLib( const std::string& unitsyncloc );
    void _FreeUnitSyncLib();

    MapInfo _GetMapInfoEx( const std::string& mapname );

    void PopulateArchiveList();

    UnitsyncImage _GetMapImage( const std::string& mapname, const std::string& imagename, UnitsyncImage (SpringUnitSyncLib::*loadMethod)(const std::string&) );
    UnitsyncImage _GetScaledMapImage( const std::string& mapname, UnitsyncImage (SpringUnitSync::*loadMethod)(const std::string&), int width, int height );

    void _GetMapImageAsync( const std::string& mapname, UnitsyncImage (SpringUnitSync::*loadMethod)(const std::string&), int evtHandlerId );

public:
	std::string GetNameForShortname( const std::string& shortname, const std::string& version ) const;
};



struct GameOptions
{
  SpringUnitSync::OptionMapBool bool_map;
  SpringUnitSync::OptionMapFloat float_map;
  SpringUnitSync::OptionMapString string_map;
  SpringUnitSync::OptionMapList list_map;
  SpringUnitSync::OptionMapSection section_map;
};

/// Helper class for managing async operations safely
class UnitSyncAsyncOps
{
  public:
	UnitSyncAsyncOps( wxEvtHandler* evtHandler )
		: m_id( usync().RegisterEvtHandler( evtHandler ) )
	{}
	~UnitSyncAsyncOps() {
	  usync().UnregisterEvtHandler( m_id );
	}

	void GetMinimap( const std::string& mapname )                 { usync().GetMinimapAsync( mapname, m_id ); }
	void GetMinimap( const std::string& mapname, int w, int h )   { usync().GetMinimapAsync( mapname, w, h, m_id ); }
	void GetMetalmap( const std::string& mapname )                { usync().GetMetalmapAsync( mapname, m_id ); }
	void GetMetalmap( const std::string& mapname, int w, int h )  { usync().GetMetalmapAsync( mapname, w, h, m_id ); }
	void GetHeightmap( const std::string& mapname )               { usync().GetHeightmapAsync( mapname, m_id ); }
	void GetHeightmap( const std::string& mapname, int w, int h ) { usync().GetHeightmapAsync( mapname, w, h, m_id ); }
	void GetMapEx( const std::string& mapname )                   { usync().GetMapExAsync( mapname, m_id ); }

  private:
	int m_id;
};

// namespace LSL

#endif // SPRINGLOBBY_HEADERGUARD_SPRINGUNITSYNC_H

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

