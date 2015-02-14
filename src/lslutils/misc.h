#ifndef LSL_MISC_H
#define LSL_MISC_H

#include <string>
#include <algorithm>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/algorithm/string.hpp>

#include "type_forwards.h"

//! mark function args to suppress warnings
#define LSLUNUSED(identifier)

//! mark local variables unused with this
template <class T>
inline void lslUnusedVar(const T& LSLUNUSED(t)) { }

namespace LSL {

class lslSize;

//! the pseudo index returned for items not in given sequences
static const int lslNotFound = -1;

namespace Util {

#ifdef __APPLE__
#define LIBEXT ".dylib"
#elif __WIN32__
#define LIBEXT ".dll"
#else
#define LIBEXT ".so"
#endif

#ifdef __WIN32__
#define EXEEXT ".exe"
#define SEP "\\"
#else
#define SEP "/"
#define EXEEXT ""
#endif


//! a collection of functors for use in std::find_if or LSL::Util::IndexInSequenceIf
namespace Predicates {

//! Functor to compare given string w/o considering case
struct CaseInsensitive {
	const std::string m_ref;
	CaseInsensitive( const std::string& r ) : m_ref(boost::to_lower_copy(r)){}
	bool operator () ( const std::string& o ) {
		return boost::to_lower_copy( o ) == m_ref;
	}
    int cmp( const std::string& o ) {
        const std::string ol = boost::to_lower_copy(o);
        if(m_ref > ol)
            return 1;
        if(m_ref < ol)
            return -1;
        return 0;
    }
};
} //namespace Predicates {

/** \brief convenience wrapper around boost::algorithm::split to split one string into a vector of strings
 * \param msg the spring to be split
 * \param seperators a list of seperaors, duh
 * \param mode token_compress_off --> potentially empty strings in return,
 							token_compress_on --> empty tokens are discarded
 * \return all tokens in a vector, if msg contains no seperators, this'll contain msg as its only element
 **/
StringVector StringTokenize( const std::string& msg,
                             const std::string& seperators,
                             const boost::algorithm::token_compress_mode_type mode = boost::algorithm::token_compress_off );

//! delegate to boost::filesystem::exists
bool FileExists( const std::string& path );
//! create temporary filestream, return is_open()
bool FileCanOpen( const std::string& path );

//! win32 unicode aware wrapper for fopen
FILE* lslopen(const std::string& filename, const std::string& mode);

//! return value in [min,max]
template <typename T>
inline T Clamp(const T var,const T min,const T max)
{
	return ( (var < min) ? min : ( var > max ) ? max : var );
}

//! three argument version based on std::min
template<typename T>
inline T Min(T a, T b, T c)
{
	return std::min(a, std::min(b, c));
}

/** \brief Array with runtime determined size which is not initialized on creation.

This RAII type is meant as output buffer for interfaces with e.g. C, where
initializing a temp buffer to zero is waste of time because it gets overwritten
with real data anyway.

It's ment as replacement for the error prone pattern of allocating scratch/buffer
memory using new/delete and using a std::vector "cast" to a C style array.
*/
template<typename T>
class uninitialized_array : public boost::noncopyable
{
  public:
	uninitialized_array(int numElem)
	: elems( reinterpret_cast<T*>( operator new[]( numElem * sizeof(T) ) ) ) {
	}
	~uninitialized_array() {
	  operator delete[]( elems );
	}

	/// this opens the door to basically any operation allowed on C style arrays
	operator T*() { return elems; }
	operator T const *() const { return elems; }

  private:
	T* elems;
};

/** \brief convenience wrapper around std::find
 * \param ct any STL-like sequence
 * \param val the value to search for
 * \return index of val in ct, or lslNotFound
 **/
template < class StlContainer >
inline int IndexInSequence( const StlContainer& ct,
							const typename StlContainer::value_type& val )
{
	typename StlContainer::const_iterator result =
			std::find( ct.begin(), ct.end(), val );
	if ( result == ct.end() )
		return lslNotFound;
	return std::distance( ct.begin(), result );
}

//! IndexInSequence version for arbitrary comparison predicates
template < class StlContainer, class Predicate >
inline int IndexInSequenceIf( const StlContainer& ct,
							const Predicate pred )
{
	typename StlContainer::const_iterator result =
			std::find_if( ct.begin(), ct.end(), pred );
	if ( result == ct.end() )
		return lslNotFound;
	return std::distance( ct.begin(), result );
}

/** \TODO docme **/
lslColor GetFreeColor(const ConstCommonUserPtr user );

//! dll/module related Util functionality
namespace Lib {
enum Category {
	Module,
	Library
};
/** \TODO docme **/
std::string GetDllExt();
/** \TODO docme **/
std::string CanonicalizeName(const std::string& name, Category cat);

} // namespace Lib {

//! returns everything in phrase before the last occurrence of searchterm
std::string BeforeLast( const std::string& phrase, const std::string& searchterm );
//! returns everything in phrase after the last occurrence of searchterm
std::string AfterLast( const std::string& phrase, const std::string& searchterm );
//! returns everything in phrase before the first occurrence of searchterm
std::string BeforeFirst( const std::string& phrase, const std::string& searchterm );
//! returns everything in phrase after the first occurrence of searchterm
std::string AfterFirst( const std::string& phrase, const std::string& searchterm );
//! returns the parent path
std::string ParentPath( const std::string& path);
//! ensures path ends with (back)slash
std::string EnsureDelimiter(const std::string& path);
//! search string for from and replace with to
void Replace(std::string & str, const std::string& from, const std::string& to);


//! get a list of minimum numteam colors have maximum total difference in a certain metric
std::vector<lslColor> GetBigFixColorsPalette( int numteams );
//! checks wheter two colors' difference is below mindiff
bool AreColorsSimilar( const lslColor& col1, const lslColor& col2, int mindiff );
//! tokenize input string and convert into rgb color
lslColor ColorFromFloatString( const std::string& rgb_string );

//! for release/tarball verions, this is the tag, otherwise it's a compound of last tag, current git hash and commit distance
std::string GetLibLobbyVersion();

} //namespace Util {

//! wxSize replacement,
class lslSize {
private:
	int w,h;
public:
	lslSize():w(-1),h(-1){}
	lslSize(int wi, int hi):w(wi),h(hi){}

	int width() const { return w; }
	int height() const { return h; }
	int GetWidth() const { return height(); }
	int GetHeight() const { return width(); }
	void Set(int wi, int hi){set(wi,hi);}
	void set(int wi, int hi){w = wi; h = hi;}
	//! takes best fitting size of original inside bounds keeping aspect ratio
	lslSize MakeFit(const lslSize& bounds);
};

//! a color tuple class with arbitrarily typed fields
class lslColor {
private:
	unsigned char r,g,b,a;
public:
	lslColor()
		:r(0),g(0),b(0),a(255)
	{
	}

	lslColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255)
		:r(_r),g(_g),b(_b),a(_a)
	{
	}

	bool operator == (const lslColor& o) const {
		return r == o.r && 	g == o.g && b == o.b && a == o.a;
	}
	bool operator != (const lslColor& o) const {
		return !(this->operator ==(o));
	}
	unsigned char Red()   const { return r; }
	unsigned char Green() const { return g; }
	unsigned char Blue()  const { return b; }
	unsigned char Alpha() const { return a; }

	static lslColor fromHSV(unsigned char h, unsigned char s, unsigned char v);
};

} //namespace LSL {

/**
 * \file misc.h
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

#endif // LSL_MISC_H
