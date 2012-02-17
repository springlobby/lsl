#ifndef LSL_CONFIG_H
#define LSL_CONFIG_H

#define STR_DUMMY(name) std::string name () const { return std::string(); }
#define INT_DUMMY(name) int name () const { return 0; }

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
    STR_DUMMY( GetCurrentUsedDataDir )
    STR_DUMMY( GetCurrentUsedSpringBinary )
    STR_DUMMY( GetCurrentUsedSpringConfigFilePath )
    STR_DUMMY( GetMyInternalUdpSourcePort )
    INT_DUMMY( GetClientPort )
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

/**
 * \file lslconfig.h
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

#endif // LSL_CONFIG_H
