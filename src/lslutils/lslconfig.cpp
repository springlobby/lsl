#include "lslconfig.h"
#include "misc.h"

namespace LSL {

Config::Config()
{
}

Config& sett() {
    static Config cc;
    return cc;
}

lslColor Config::GetBattleLastColor() const { return lslColor(); }

}// namespace LSL {
