#include "misc.h"

#include <boost/filesystem.hpp>
#include <fstream>

namespace LSL {
namespace Util {

bool FileExists( const std::string path )
{
    return boost::filesystem::exists( boost::filesystem::path( path ) );
}

bool FileCanOpen( const std::string path )
{
	return std::ifstream(path.c_str()).is_open();
}

namespace Lib {

std::string GetDllExt()
{
#if defined(__UNIX__)
	return ".so";
#elif defined(WIN32)
	return ".dll";
#else
	assert( false );
#endif
}

std::string CanonicalizeName( const std::string& name, Category cat)
{
	std::string nameCanonic;
	// under Unix the library names usually start with "lib" prefix, add it
#if defined(__UNIX__) && !defined(__EMX__)
	switch ( cat )
	{
		default:
			wxFAIL_MSG( _T("unknown wxDynamicLibraryCategory value") );
			// fall through
		case wxDL_MODULE:
			// don't do anything for modules, their names are arbitrary
			break;
		case wxDL_LIBRARY:
			// library names should start with "lib" under Unix
			nameCanonic = _T("lib");
			break;
	}
#else // !__UNIX__
	lslUnusedVar(cat);
#endif // __UNIX__/!__UNIX__
	nameCanonic + name + GetDllExt();
	return nameCanonic;
}

} //namespace Lib

std::string BeforeLast( const std::string& phrase, const std::string& searchterm )
{
	const size_t pos = phrase.rfind( searchterm );
	return phrase.substr( 0, pos );
}

std::string AfterLast( const std::string& phrase, const std::string& searchterm )
{
	const size_t pos = phrase.rfind( searchterm );
	return phrase.substr( pos+1 );
}

std::string BeforeFirst( const std::string& phrase, const std::string& searchterm )
{
	const size_t pos = phrase.find( searchterm );
	return phrase.substr( 0, pos );
}

std::string AfterFirst( const std::string& phrase, const std::string& searchterm )
{
	const size_t pos = phrase.find( searchterm );
	return phrase.substr( pos+1 );
}

} //namespace Util


lslSize lslSize::MakeFit(const lslSize bounds)
{
	if( ( bounds.GetWidth() <= 0 ) || ( bounds.GetHeight() <= 0 ) )
		return lslSize(0,0);
	const int sizex = ( this->GetWidth() * bounds.GetHeight() ) / this->GetHeight();
	if( sizex <= bounds.GetWidth() )
	{
	  return lslSize( sizex, bounds.GetHeight() );
	}
	else
	{
	  const int sizey = ( this->GetHeight() * bounds.GetWidth() ) / this->GetWidth();
	  return lslSize( bounds.GetWidth(), sizey );
	}
}

} //namespace LSL
