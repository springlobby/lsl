/* This file is part of the Springlobby (GPL v2 or later), see COPYING */

#include "config.h"

#include <lslutils/conversion.h>

#include "misc.h"
#include "globalsmanager.h"

namespace LSL
{
namespace Util
{

// FIXME chose better defaults
Config::Config()
    : Cache("cache")
    , CurrentUsedUnitSync("unitsync")
    , CurrentUsedSpringBinary("spring")
{
}

Config& config()
{
	static LSL::Util::LineInfo<Config> m(AT);
	static LSL::Util::GlobalObjectHolder<Config, LSL::Util::LineInfo<Config> > m_sett(m);
	return m_sett;
}

lslColor Config::GetBattleLastColor() const
{
	//    auto
	return lslColor();
}

std::string Config::GetCachePath() const
{
	return Cache;
}

std::string Config::GetCurrentUsedUnitSync() const
{
	return CurrentUsedUnitSync;
}


std::string Config::GetCurrentUsedSpringBinary() const
{
	return CurrentUsedSpringBinary;
}

void Config::ConfigurePaths(const std::string& Cache, const std::string& CurrentUsedUnitSync, const std::string& CurrentUsedSpringBinary)
{
	this->Cache = Cache;
	this->CurrentUsedUnitSync = CurrentUsedUnitSync;
	this->CurrentUsedSpringBinary = CurrentUsedSpringBinary;
}

} // namespace Util
} // namespace LSL {
