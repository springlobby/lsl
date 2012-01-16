#include "misc.h"
#include "conversion.h"

#include <boost/filesystem.hpp>
#include <fstream>

namespace LSL {
namespace Util {

std::string GetHostCPUSpeed()
{
	return "667";
}

std::string GetLibLobbyVersion()
{
	return "0";
}

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

bool AreColorsSimilar( const lslColor& col1, const lslColor& col2, int mindiff )
{
	int r,g,b;
	r = std::abs( col1.Red() - col2.Red() );
	g = std::abs( col1.Green() - col2.Green() );
	b = std::abs( col1.Blue() - col2.Blue() );
	int difference = std::min( r, g );
	difference = std::min( difference, b );
	return difference < mindiff;
}

typedef std::vector<double> huevec;

void hue(huevec& out, int amount, int level)
{
	if (level <= 1) {
		if (long(out.size()) < amount)
			out.push_back(0.0);
		if (long(out.size()) < amount)
			out.push_back(0.5);
	}
	else {
		hue(out, amount, level - 1);
		const int lower = out.size();
		hue(out, amount, level - 1);
		const int upper = out.size();
		for (int i = lower; i < upper; ++i)
			out.at(i) += 1.0 / (1 << level);
	}
}

void hue(huevec& out, int amount)
{
	int level = 0;
	while ((1 << level) < amount) ++level;

	out.reserve(amount);
	hue(out, amount, level);
}

std::vector<lslColor>& GetBigFixColorsPalette( int numteams )
{
	static std::vector<lslColor> result;
	huevec huevector;
	static int satvalbifurcatepos;
	static std::vector<double> satvalsplittings;
	if ( satvalsplittings.empty() ) // insert ranges to bifurcate
	{
		satvalsplittings.push_back( 1 );
		satvalsplittings.push_back( 0 );
		satvalbifurcatepos = 0;
	}
	hue( huevector, numteams );
	int bisectionlimit = 20;
	for ( int i = result.size(); i < numteams; i++ )
	{
		double hue = huevector[i];
		double saturation = 1;
		double value = 1;
		int switccolors = i % 3; // why only 3 and not all combinations? because it's easy, plus the bisection limit cannot be divided integer by it

		if ( ( i % bisectionlimit ) == 0 )
		{
			satvalbifurcatepos = satvalbifurcatepos % ( satvalsplittings.size() -1 );
			std::vector<double>::iterator toinsert = satvalsplittings.begin() + satvalbifurcatepos + 1;
			satvalsplittings.insert( toinsert, ( satvalsplittings[satvalbifurcatepos] - satvalsplittings[satvalbifurcatepos + 1] ) / 2 + satvalsplittings[satvalbifurcatepos + 1] );
			satvalbifurcatepos += 2;
		}

		if ( switccolors == 1 )
		{
			saturation = satvalsplittings[satvalbifurcatepos -1];
		}
		else if ( switccolors == 2 )
		{
			value = satvalsplittings[satvalbifurcatepos -1];
		}
		hue += 0.17; // use as starting point a zone where color band is narrow so that small variations means high change in visual effect
		if ( hue > 1 ) hue-= 1;
//		wxImage::HSVValue hsvcolor( hue, saturation, value );
//		wxImage::RGBValue rgbvalue = wxImage::HSVtoRGB( hsvcolor );
//		lslColor col( rgbvalue.red, rgbvalue.green, rgbvalue.blue );
		assert( false );//conversion needs implementing
		lslColor col;
		result.push_back( col );
	}
	return result;
}

lslColor ColorFromFloatString(const std::string &rgb_string)
{
    const StringVector values = Util::StringTokenize( rgb_string, " " );
    unsigned char decimal_colors[3];
    for ( size_t i = 0; i < 3; ++i) {
        const double value = values.size() > i ? Util::FromString<double>( values[i] ) : 0.0;
        decimal_colors[i] = Clamp( static_cast<unsigned char>(value*256), static_cast<unsigned char>(0), static_cast<unsigned char>(255) );
    }
    return lslColor( decimal_colors[0], decimal_colors[1], decimal_colors[2] );
}

lslColor GetFreeColor( const ConstUserPtr /*user*/ )
{
    return lslColor();
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
