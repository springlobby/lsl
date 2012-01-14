#ifndef LSL_MISC_H
#define LSL_MISC_H

#include <string>
#include <algorithm>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/algorithm/string.hpp>

#include "type_forwards.h"

#define LSLUNUSED(identifier)

template <class T>
inline void lslUnusedVar(const T& LSLUNUSED(t)) { }

namespace LSL {

class lslSize;
template < class T > class lslColorBase;
typedef lslColorBase<unsigned char> lslColor;

static const int lslNotFound = -1;

namespace Util {

namespace Predicates {
struct CaseInsensitive {
	const std::string m_ref;
	CaseInsensitive( const std::string r ) : m_ref(boost::to_lower_copy(r)){}
	bool operator () ( const std::string& o ) {
		return boost::to_lower_copy( o ) == m_ref;
	}
};
} //namespace Predicates {

StringVector StringTokenize( const std::string& msg,
                             const std::string& seperators,
                             const boost::algorithm::token_compress_mode_type mode = boost::algorithm::token_compress_off )
{
    StringVector strings;
    boost::algorithm::split( strings, msg, boost::algorithm::is_any_of(seperators),
                             mode );
    return strings;
}

//! delegate to boost::filesystem::exists
bool FileExists( const std::string path );
//! create temporary filestream, return is_open()
bool FileCanOpen( const std::string path );

template <typename T> T Clamp(const T var,const T min,const T max)
{
	return ( (var < min) ? min : ( var > max ) ? max : var );
}

template<typename T>
T Min(T a, T b, T c)
{
	return std::min(a, std::min(b, c));
}

/** @brief Array with runtime determined size which is not initialized on creation.

This RAII type is ment as output buffer for interfaces with e.g. C, where
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

namespace Lib {
enum Category {
	Module,
	Library
};
std::string GetDllExt();
std::string CanonicalizeName(const std::string& name, Category cat);

} // namespace Lib {

std::string BeforeLast( const std::string& phrase, const std::string& searchterm );
std::string AfterLast( const std::string& phrase, const std::string& searchterm );
std::string BeforeFirst( const std::string& phrase, const std::string& searchterm );
std::string AfterFirst( const std::string& phrase, const std::string& searchterm );

std::vector<lslColor>& GetBigFixColoursPalette( int numteams );
bool AreColoursSimilar( const lslColor& col1, const lslColor& col2, int mindiff );

std::string GetHostCPUSpeed();
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
	lslSize MakeFit(const lslSize bounds);
};

template < typename T = unsigned char >
class lslColorBase {
private:
	T r,g,b,a;
public:
	lslColorBase():r(0),g(0),b(0),a(0){}
	explicit lslColorBase(T _r, T _g, T _b, T _a = 0)
		:r(_r),g(_g),b(_b),a(_a){}

	bool operator == (const lslColor o) const {
		return r == o.r && 	g == o.g && b == o.b && a == o.a;
	}
	bool operator != (const lslColor o) const {
		return !(this->operator ==(o));
	}
	T Red()   const { return r; }
	T Green() const { return g; }
	T Blue()  const { return b; }

};

} //namespace LSL {

#endif // LSL_MISC_H
