#ifndef TASSERVER_H
#define TASSERVER_H

namespace LSL {

class CommandDictionary;

class TASServer {
public:
    TASServer(int TASServerMode);

private:
    void OnNewUser( const std::string& nick, const std::string& country, int cpu, int id );

    friend class CommandDictionary;
    CommandDictionary* m_cmd_dict;
};

} //namespace LSL

#endif // TASSERVER_H
