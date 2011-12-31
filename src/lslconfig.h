#ifndef CONFIG_H
#define CONFIG_H

#define STR_DUMMY(name) std::string name () const { return std::string(); }

namespace LSL {

class Config
{
public:
    Config();
    STR_DUMMY( GetCachePath )
    STR_DUMMY( GetForcedSpringConfigFilePath )
};

Config& sett() {
    static Config cc;
    return cc;
}

} // namespace LSL

#endif // CONFIG_H
