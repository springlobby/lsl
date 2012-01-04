#ifndef TASSERVER_H
#define TASSERVER_H

#include "iserver.h"

namespace boost {
class format;
}

namespace LSL {

class CommandDictionary;

class TASServer : public iServer {
public:
    TASServer();

private:
	void OnNewUser( std::string nick, std::string country, int cpu, int id );
    void ExecuteCommand( const std::string& cmd, const std::string& inparams, int replyid = -1);
	void SendCmd( const std::string& command, const std::string& param );
	void SendCmd( const std::string& command, const boost::format& param );
	virtual void JoinChannel( const std::string& channel, const std::string& key );
	virtual void PartChannel( ChannelPtr channel );

	virtual void DoActionChannel( const ChannelPtr channel, const std::string& msg );
	virtual void SayChannel( const ChannelPtr channel, const std::string& msg );

	virtual void SayPrivate( const UserPtr user, const std::string& msg );
	virtual void DoActionPrivate( const UserPtr user, const std::string& msg );

	virtual void SayBattle( const BattlePtr battle, const std::string& msg );
	virtual void DoActionBattle( const BattlePtr battle, const std::string& msg );

	virtual void Ring( const UserPtr user );

    friend class CommandDictionary;
    CommandDictionary* m_cmd_dict;
};

} //namespace LSL

#endif // TASSERVER_H
