#ifndef LSL_MISC_H
#define LSL_MISC_H

#include <string>
#include <algorithm>
#include <boost/noncopyable.hpp>
#include <boost/algorithm/string/find.hpp>

#define LSLUNUSED(identifier)

template <class T>
inline void lslUnusedVar(const T& LSLUNUSED(t)) { }


namespace LSL {

class lslSize;
static const int lslNotFound = -1;

namespace Util {

//! delegate to boost::filesystem::exists
bool FileExists( const std::string path );
//! create temporary filestream, return is_open()
bool FileCanOpen( const std::string path );

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

namespace Lib {
enum Category {
	Module,
	Library
};
std::string GetDllExt();
std::string CanonicalizeName(const std::string& name, Category cat);

} // namespace Lib {

static std::string BeforeLast( const std::string& phrase, const std::string& searchterm )
{
	const size_t pos = phrase.rfind( searchterm );
	return phrase.substr( 0, pos );
}

static std::string AfterLast( const std::string& phrase, const std::string& searchterm )
{
	const size_t pos = phrase.rfind( searchterm );
	return phrase.substr( pos+1 );
}

static std::string BeforeFirst( const std::string& phrase, const std::string& searchterm )
{
	const size_t pos = phrase.find( searchterm );
	return phrase.substr( 0, pos );
}

static std::string AfterFirst( const std::string& phrase, const std::string& searchterm )
{
	const size_t pos = phrase.find( searchterm );
	return phrase.substr( pos+1 );
}

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

} //namespace LSL {

#endif // LSL_MISC_H
