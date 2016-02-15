/* This file is part of the Springlobby (GPL v2 or later), see COPYING */

#ifndef LIBSPRINGLOBBY_HEADERGUARD_SPRINGUNITSYNC_CACHE_H
#define LIBSPRINGLOBBY_HEADERGUARD_SPRINGUNITSYNC_CACHE_H

#include <string>
#include <lslutils/type_forwards.h>

namespace LSL
{

struct MapInfo;
struct GameOptions;

namespace Cache
{
//! returns an array where each element is a line of the file
bool Get(const std::string& path, MapInfo& ret);
bool Get(const std::string& path, GameOptions& opt);
bool Get(const std::string& path, StringVector& opt);
//! write a file where each element of the array is a line
void Set(const std::string& path, const MapInfo& data);
void Set(const std::string& path, const GameOptions& opt);
void Set(const std::string& path, const StringVector& opt);

} // namespace Cache
} // namespace LSL

#endif // LIBSPRINGLOBBY_HEADERGUARD_SPRINGUNITSYNC_CACHE_H
