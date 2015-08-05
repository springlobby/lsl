#ifndef LSL_CONFIG_H
#define LSL_CONFIG_H

#define STR_DUMMY(name) std::string name () const { return std::string(); }
#define INT_DUMMY(name) int name () const { return 0; }

#include <string>
#include <boost/filesystem/path.hpp>
#include <lslutils/type_forwards.h>

namespace LSL {
namespace Util {

template <class PB, class I >
class GlobalObjectHolder;

struct SettStartBox
{
    int ally;
    int topx;
    int topy;
    int bottomx;
    int bottomy;
};

class Config
{
	Config();

private:
	std::string Cache;
	std::string CurrentUsedUnitSync;
	std::string CurrentUsedSpringBinary;

public:
	std::string GetCachePath() const;
	std::string GetCurrentUsedUnitSync() const;
	std::string GetCurrentUsedSpringBinary() const;
	void ConfigurePaths(const std::string& Cache, const std::string& CurrentUsedUnitSync, const std::string& CurrentUsedSpringBinary);
	STR_DUMMY( GetMyInternalUdpSourcePort )
	INT_DUMMY( GetClientPort )

	StringVector GetPresetList(){StringVector tmp; return tmp;}
	StringMap GetHostingPreset( const std::string&, size_t ) {StringMap tmp; return tmp;}
	void SetHostingPreset( const std::string&, size_t, const StringMap& ){}
	lslColor GetBattleLastColor() const;
	int GetBattleLastSideSel( const std::string& /*gamename*/ ) const {
		return 0;
	}
	void SaveSettings() {}
	void DeletePreset( const std::string& /*gamename*/ ) {}

	void SetMapLastStartPosType( const std::string& , const std::string&  ) {};
	std::string GetMapLastStartPosType( const std::string&) const {
		return "";
	}

	template < class T >
	void SetMapLastRectPreset( const std::string&, const T&) {}
	template < class T > T GetMapLastRectPreset( const std::string& ) {
		return T();
	}

	bool GetBattleLastAutoAnnounceDescription() const {
		return false;
	}
	int GetBattleLastAutoSpectTime() const {
		return 0;
	}

	template <class PB, class I >
	friend class GlobalObjectHolder;

};

Config& config();

} // namespace Util
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
