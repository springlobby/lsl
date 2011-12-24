#ifndef COMMANDS_H
#define COMMANDS_H

#include <boost/signals2.hpp>
#include <string>
#include <assert.h>

template < class ReturnType, class T >
ReturnType FromString(const T s)
{
    std::stringstream ss;
//    ss << s;
    ReturnType r;
    ss >> r;
    return r;
}

std::string GetParamByChar( std::string& params, const char sep )
{
    std::string::size_type pos = params.find( sep );
    std::string ret;
    if ( pos != -1 )
    {
        ret = params.substr( 0,pos );//inclusive??
        params = params.substr( pos + 1 );
    }
    else
    {
        ret = params;
        params = "";
    }
    return ret;
}

std::string GetWordParam( std::string& params )
{
    return GetParamByChar( params, ' ');
}


std::string GetSentenceParam( std::string& params )
{
   return GetParamByChar( params, '\t' );
}


long GetIntParam( std::string& params )
{
    const std::string d = GetParamByChar( params, ' ');
   return 1;//FromString( d );
}



//bool GetBoolParam( std::string& params )
//{
//    return (bool)GetIntParam( params );
//}


namespace Tokens {
    struct Word {
        typedef std::string real_type;
        const std::string value;
        Word( std::string& params )
            :value( GetWordParam( params ) ){}
    };
    struct Sentence{
        const std::string value;
        typedef std::string real_type;
        Sentence( std::string& params )
            :value( GetSentenceParam( params ) ){}
    } ;
    struct Int{
        const int value;
        typedef int real_type;
        Int( std::string& params )
            :value( GetIntParam( params ) ){}
    } ;
    struct none{};
}

struct CommandBase {
    virtual void operator () ( std::string params )
    {
        assert( false ); //means we've called a non-mapped command
    }
    template < class mtuple >
    mtuple parse( std::string params )
    {

    }
};

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
//X x;
//f =
//f(5); // Call x.foo(5)

struct Server {
    Server();
    void ExecuteCommand( const std::string& cmd, const std::string& inparams, int replyid );
    void OnNewUser(const std::string& nick, const std::string& country, const int &cpu, const int &id );
    std::map<std::string,CommandBase> cmd_map_;
};

#include <boost/tuple/tuple.hpp>
#include <boost/function.hpp>
template < class T1 = Tokens::none, class T2 = Tokens::none, class T3 = Tokens::none, class T4 = Tokens::none >
struct Command : public CommandBase  {
    typedef boost::tuples::tuple<T1,T2,T3,T4> mtuple;
    typedef boost::function< void (const typename T1::real_type&,
                                   const typename T2::real_type&,
                                   const typename T3::real_type&,
                                   const typename T4::real_type&) > CallBackType; //call traits ftw
    CallBackType func;
    template < class F, class T >
    Command( F f, T* x)
        :func ( boost::bind(f, x) )
    {}
    virtual void operator () ( std::string params )
    {

    }
};



Server::Server()
{
    cmd_map_["ADDUSER"] = Command<Tokens::Word,Tokens::Word,Tokens::Int,Tokens::Int>(&Server::OnNewUser, this);
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

void Server::OnNewUser( const std::string& nick, const std::string& country, const int& cpu, const int& id )
{
//    User* user = GetUser( id );
//    if ( !user ) user = AddUser( id );
//    user->SetCountry( country );
//    user->SetCpu( cpu );
//    user->SetNick( nick );
}

#endif // COMMANDS_H
