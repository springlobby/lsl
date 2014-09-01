#ifndef LSL_HEADERGUARD_MRU_CACHE_H
#define LSL_HEADERGUARD_MRU_CACHE_H

#include <lslutils/debug.h>

#include <string>
#include <boost/thread/mutex.hpp>
#include <list>
#include <map>

namespace LSL {

/// Thread safe MRU cache (works like a std::map but has maximum size)
template<typename TKey, typename TValue>
class MostRecentlyUsedCache
{
public:
	//! name parameter might be used to identify stats in dgb output
	MostRecentlyUsedCache(int max_size, const std::string& name = "" )
		: m_size(0),
		m_max_size(max_size),
		m_cache_hits(0),
		m_cache_misses(0),
		m_name(name)
	{
	}

	~MostRecentlyUsedCache()
	{
		LslDebug( "%s - cache hits: %d misses: %d", m_name.c_str(), m_cache_hits, m_cache_misses );
	}

	void Add( const TKey& name, const TValue& img )
	{
		boost::mutex::scoped_lock lock(m_lock);
		while ( m_size >= m_max_size ) {
			--m_size;
			m_iterator_map.erase( m_items.back().first );
			m_items.pop_back();
		}
		++m_size;
		m_items.push_front( CacheItem( name, img ) );
		m_iterator_map[name] = m_items.begin();
	}

	bool TryGet( const TKey& name, TValue& img )
	{
		boost::mutex::scoped_lock lock(m_lock);
		typename IteratorMap::iterator it = m_iterator_map.find( name );
		if ( it == m_iterator_map.end() ) {
			++m_cache_misses;
			return false;
		}
		// reinsert at front, so that most recently used items are always at front
		m_items.push_front( *it->second );
		m_items.erase( it->second );
		it->second = m_items.begin();
		// return image
		img = it->second->second;
		++m_cache_hits;
		return true;
	}

	void Clear()
	{
		boost::mutex::scoped_lock lock(m_lock);
		m_size = 0;
		m_items.clear();
		m_iterator_map.clear();
	}

private:
	typedef std::pair<TKey, TValue> CacheItem;
	typedef std::list<CacheItem> CacheItemList;
	typedef std::map<TKey, typename CacheItemList::iterator> IteratorMap;

	mutable boost::mutex m_lock;
	CacheItemList m_items;
	IteratorMap m_iterator_map;
	int m_size;
	const int m_max_size;
	int m_cache_hits;
	int m_cache_misses;
	const std::string m_name;
};

class UnitsyncImage;
struct MapInfo;
typedef MostRecentlyUsedCache<std::string,UnitsyncImage> MostRecentlyUsedImageCache;
typedef MostRecentlyUsedCache<std::string,MapInfo> MostRecentlyUsedMapInfoCache;
typedef MostRecentlyUsedCache<std::string,std::vector<std::string> > MostRecentlyUsedArrayStringCache;

} // namespace LSL

/**
 * \file mru_cache.h
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

#endif // MRU_CACHE_H
