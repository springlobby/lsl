#ifndef COMMANDS_H
#define COMMANDS_H

#include <boost/signals2.hpp>
#include <string>

struct ServerEvents {
    typedef boost::signals2::signal<void ()> BattleSigType;
    static BattleSigType battleSig;
};

struct User
{
    User()
    {
        ServerEvents::battleSig.connect( *this );
    }
  void operator()() const
  {
    std::cout << "I was called" << std::endl;
  }
};

struct Command {
};

struct Server {
    Server();
    void ExecuteCommand( const std::string& cmd, const std::string& inparams, int replyid );

  std::map<std::string,Command> cmd_map_;
};

Server::Server()
{
}

void Server::ExecuteCommand(const std::string &cmd, const std::string &inparams, int replyid)
{
    std::string params = inparams;
    int pos, cpu, id, nat, port, maxplayers, rank, specs, units, top, left, right, bottom, ally, type;
    bool haspass,lanmode = false;
    std::string hash;
    std::string nick, contry, host, map, title, mod, channel, error, msg, owner, ai, supported_spring_version, topic;
    //NatType ntype;
//    UserStatus cstatus;
//    UTASClientStatus tasstatus;
//    UTASBattleStatus tasbstatus;
//    UserBattleStatus bstatus;
//    UTASColor color;


}

#endif // COMMANDS_H
