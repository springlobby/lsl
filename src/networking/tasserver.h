#ifndef TASSERVER_H
#define TASSERVER_H

namespace LSL {

class CommandDictionary;

class TASServer {
public:
    TASServer();

private:
    void OnNewUser( const std::string& nick, const std::string& country, int cpu, int id );
    void ExecuteCommand( const std::string& cmd, const std::string& inparams, int replyid = -1);

    friend class CommandDictionary;
    CommandDictionary* m_cmd_dict;
};

} //namespace LSL

#endif // TASSERVER_H
