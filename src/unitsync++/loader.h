#ifndef LSL_LOADER_H
#define LSL_LOADER_H

namespace LSL {

class UnitsyncLib;

struct UnitsyncFunctionLoader
{
	static void LuaParser( UnitsyncLib* s );
	static void MMOptions( UnitsyncLib* s );
	static void Map( UnitsyncLib* s );
	static void Mod( UnitsyncLib* s );
	static void Basic( UnitsyncLib* s );
	static void Config( UnitsyncLib* s );
};

} // namespace LSL

#endif // LSL_LOADER_H
