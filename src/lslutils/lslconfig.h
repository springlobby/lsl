#ifndef LSL_CONFIG_H
#define LSL_CONFIG_H

/** \file lslconfig.h
    \copyright GPL v2 **/

#define STR_DUMMY(name) std::string name () const { return std::string(); }

#include <string>
#include <lslutils/type_forwards.h>

namespace LSL {

namespace Settings {
struct SettStartBox
{
    int ally;
    int topx;
    int topy;
    int bottomx;
    int bottomy;
};
} // namespace Settings {

class Config
{
public:
    Config();
    STR_DUMMY( GetCachePath )
    STR_DUMMY( GetForcedSpringConfigFilePath )
    STR_DUMMY( GetCurrentUsedUnitSync )
    std::string GetCurrentUsedUnitSync() { return "/usr/lib/spring/libunitsync.so";}
	StringVector GetPresetList() { return StringVector(); }
    StringMap GetHostingPreset( const std::string&, size_t ) { return StringMap(); }
    void SetHostingPreset( const std::string&, size_t, const StringMap& ) {  }
    lslColor GetBattleLastColor() const;
    int GetBattleLastSideSel( const std::string& /*modname*/ ) const;
    void SaveSettings(){}
    void DeletePreset( const std::string& /*modname*/ ) {}

    void SetMapLastStartPosType( const std::string& , const std::string&  ){}
    std::string GetMapLastStartPosType( const std::string&){ return std::string(); }


    template < class T >
    void SetMapLastRectPreset( const std::string&, const T&) {}
    template < class T > T GetMapLastRectPreset( const std::string& ) { return T(); }

    bool GetBattleLastAutoAnnounceDescription() { return true; }
    int GetBattleLastAutoSpectTime() {return 1;}
};

Config& sett();

} // namespace LSL

#endif // LSL_CONFIG_H
