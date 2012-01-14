#ifndef CONFIG_H
#define CONFIG_H

#define STR_DUMMY(name) std::string name () const { return std::string(); }

#include <string>
#include <lslutils/type_forwards.h>

namespace LSL {

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
};

Config& sett();

} // namespace LSL

#endif // CONFIG_H
