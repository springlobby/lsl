#include "lslconfig.h"

namespace LSL {

Config::Config()
{
}

Config& sett() {
    static Config cc;
    return cc;
}


}// namespace LSL {
