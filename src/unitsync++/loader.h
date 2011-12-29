#ifndef LSL_LOADER_H
#define LSL_LOADER_H

namespace LSL {

class SpringUnitSyncLib;

struct UnitsyncFunctionLoader
{
    static void LuaParser( SpringUnitSyncLib* s );
    static void MMOptions( SpringUnitSyncLib* s );
    static void Map( SpringUnitSyncLib* s );
    static void Mod( SpringUnitSyncLib* s );
    static void Basic( SpringUnitSyncLib* s );
    static void Config( SpringUnitSyncLib* s );
};

} // namespace LSL

#endif // LSL_LOADER_H
