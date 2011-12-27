#ifndef ISERVER_PRIVATE_H
#define ISERVER_PRIVATE_H

#include <string>
#include <sstream>

#include <boost/tuple/tuple.hpp>
#include <boost/function.hpp>

#include "tasserver.h"

namespace LSL {

template < class ReturnType, class T >
ReturnType FromString(const T s)
{
    std::stringstream ss;
    ss << s;
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
   return FromString<long>( d );
}

bool GetBoolParam( std::string& params )
{
    return (bool)GetIntParam( params );
}

namespace Tokens {

template < class TypeImp >
struct Basic {
    typedef TypeImp
        real_type;
    const real_type value;

    Basic( real_type v )
        :value(v)
    {}

    real_type operator ()() const
    { return value; }
};
struct Word : public Basic<std::string>{
    Word( std::string& params )
        :Basic<std::string>( GetWordParam( params ) ){}
};
struct Sentence : public Basic<std::string> {
    Sentence( std::string& params )
        :Basic<std::string>( GetWordParam( params ) ){}
} ;
struct Int : public Basic<int>{
    Int( std::string& params )
        :Basic<int>( GetIntParam( params ) ){}
} ;
struct Float: public Basic<float>{
    Float( std::string& params )
        :Basic<float>( GetIntParam( params ) ){}
} ;
struct Double: public Basic<double>{
    Double( std::string& params )
        :Basic<double>( GetIntParam( params ) ){}
} ;
struct none{
    template < class T > none( T& ){}
};

} //namespace Tokens

namespace Commands {

struct CommandBase {
    virtual void process( std::string& params )
    {
        assert( false ); //means we've called a non-mapped command
    }
};

template < class T1 , class T2 , class T3 , class T4, class T5 >
struct Signature {
    typedef boost::function< void (const typename T1::real_type&,
                                   const typename T2::real_type&,
                                   const typename T3::real_type&,
                                   const typename T4::real_type&,
                                   const typename T5::real_type&) > Type; //call traits ftw
    template < class F, class T >
    static Type make( F f, T* x )
    {
        return Type( boost::bind( f, x, _1, _2, _3, _4, _5 ) );
    }
    static void call( Type& func, std::string& params )
    {
        BOOST_AUTO(t1,T1( params )());
        BOOST_AUTO(t2,T2( params )());
        BOOST_AUTO(t3,T3( params )());
        BOOST_AUTO(t4,T4( params )());
        BOOST_AUTO(t5,T5( params )());
        func( t1, t2, t3, t4, t5 );
    }
};

template < class T1 , class T2 , class T3 , class T4 >
struct Signature< T1,T2,T3,T4, Tokens::none > {
    typedef boost::function< void (const typename T1::real_type&,
                                   const typename T2::real_type&,
                                   const typename T3::real_type&,
                                   const typename T4::real_type&) > Type; //call traits ftw
    template < class F, class T >
    static Type make( F f, T* x )
    {
        return Type( boost::bind( f, x, _1, _2, _3, _4 ) );
    }

    static void call( Type& func, std::string& params )
    {
        BOOST_AUTO(t1,T1( params )());
        BOOST_AUTO(t2,T2( params )());
        BOOST_AUTO(t3,T3( params )());
        BOOST_AUTO(t4,T4( params )());

        func( t1, t2, t3, t4 );
    }
};

template < class T1 = Tokens::none, class T2 = Tokens::none, class T3 = Tokens::none, class T4 = Tokens::none, class T5 = Tokens::none >
struct Command : public CommandBase  {
    typedef boost::tuples::tuple<T1,T2,T3,T4,T5> mtuple;
    typedef Signature<T1,T2,T3,T4,T5> SignatureType;
    typedef typename SignatureType::Type CallBackType;
    CallBackType func;
    template < class F, class T >
    Command( F f, T* x)
        :func ( SignatureType::make( f, x ) )
    {}
    virtual void process( std::string& params )
    {
        SignatureType::call( func, params );
    }
};

} //namespace Commands

#define NEWCMD(Name,Func,T1,T2,T3,T4) \
    cmd_map_[Name] = boost::shared_ptr<Commands::Command<Tokens::T1,Tokens::T2,Tokens::T3,Tokens::T4> > \
    ( new Commands::Command<Tokens::T1,Tokens::T2,Tokens::T3,Tokens::T4>(&TASServer::Func, this) )

struct CommandDictionary {
    CommandDictionary()
    {
        NEWCMD("ADDUSER",OnNewUser,Word,Word,Int,Int);
        NEWCMD("TASSERVER",OnConnected,Word,Int,Word,Int);
        NEWCMD("ACCEPTED",OnLogin,Word);
        NEWCMD("MOTD",OnMotd,All);
        NEWCMD("ADDUSER",OnNewUser,Word,Word,Int,Int);
        NEWCMD("CLIENTSTATUS",OnUserStatusChanged,Word,Int);
        NEWCMD("BATTLEOPENED",OnBattleOpened,Int,Int,Int,Word,Word,Int,Int,Int,Int,Word,Sentence,Sentence,Sentence,Sentence);
        NEWCMD("JOINEDBATTLE",OnUserJoinedBattle,Int,Word,Word);
        NEWCMD("UPDATEBATTLEINFO",OnBattleInfoUpdated,Int,Int,Int,Word,Sentence);
        NEWCMD("LOGININFOEND",OnLoginInfoComplete);
        NEWCMD("REMOVEUSER",OnUserQuit,Word);
        NEWCMD("BATTLECLOSED",OnBattleClosed,Int);
        NEWCMD("LEFTBATTLE",OnUserLeftBattle,Int,Word);
        NEWCMD("JOIN",OnJoinChannel,Word,Sentence);
        NEWCMD("SAID",OnChannelSaid,Word,Word,All);
        NEWCMD("JOINED",OnChannelSaid,Word,Word);
        NEWCMD("LEFT",OnChannelPart,Word,Word,All);
        NEWCMD("CHANNELTOPIC",OnChannelTopic,Word,Word,Int,All);
        NEWCMD("SAIDEX",OnChannelAction,Word,Word,All);
        NEWCMD("CLIENTS",OnChannelJoinUserList,Word,All);
        NEWCMD("SAYPRIVATE",OnPrivateMessage,Word,All);
        NEWCMD("SAYPRIVATEEX",OnSayPrivateMessageEx,Word,All);
        NEWCMD("SAIDPRIVATEEX",OnSaidPrivateMessageEx,Word,All);
        NEWCMD("JOINBATTLE",OnJoinedBattle,Int,Word);
        NEWCMD("CLIENTBATTLESTATUS",OnClientBattleStatus,Word,Int,Int);
        NEWCMD("ADDSTARTRECT",OnBattleStartRectAdd,Int,Int,Int,Int,Int);
        NEWCMD("REMOVESTARTRECT",OnBattleStartRectRemove,Int);
        NEWCMD("ENABLEALLUNITS",OnBattleEnableAllUnits);
        NEWCMD("ENABLEUNITS",OnBattleEnableUnits,All);
        NEWCMD("DISABLEUNITS",OnBattleDisableUnits,All);
        NEWCMD("CHANNEL",OnChannelListEntry,Word,Int,All);
        NEWCMD("ENDOFCHANNELS",OnChannelListEnd);
        NEWCMD("REQUESTBATTLESTATUS",OnRequestBattleStatus);
        NEWCMD("SAIDBATTLE",OnSaidBattle,Word,All);
        NEWCMD("SAIDBATTLEEX",OnBattleAction,Word,All);
        NEWCMD("AGREEMENT",OnAgreenmentLine,All);
        NEWCMD("AGREEMENTEND",OnAcceptAgreement);
        NEWCMD("OPENBATTLE",OnHostedBattle,Int);
        NEWCMD("ADDBOT",OnBattleAddBot,Int,Word,Word,Int,Int,All);
        NEWCMD("UPDATEBOT",OnBattleUpdateBot,Int,Word,Int,Int);
        NEWCMD("REMOVEBOT",OnBattleRemoveBot,Int,Word);
        NEWCMD("RING",OnRing,Word);
        NEWCMD("SERVERMSG",OnServerMessage,All);
        NEWCMD("JOINBATTLEFAILED",OnJoinBattleFailed,All);
        NEWCMD("OPENBATTLEFAILED",OnOpenBattleFailed,All);
        NEWCMD("JOINFAILED",OnJoinChannelFailed,All);
        NEWCMD("ACQUIREUSERID",OnGetHandle);
        NEWCMD("FORCELEAVECHANNEL",OnKickedFromChannel,Word,Word,All);
        NEWCMD("DENIED",OnLoginFailed,All);
        NEWCMD("HOSTPORT",OnHostExternalUdpPort,Int);
        NEWCMD("UDPSOURCEPORT",OnMyExternalUdpSourcePort,Int);
        NEWCMD("CLIENTIPPORT",OnClientIPPort,Word,Word,Int);
        NEWCMD("SETSCRIPTTAGS",OnSetBattleInfo,All);
        NEWCMD("SCRIPTSTART",OnScriptStart);
        NEWCMD("SCRIPTEND",OnScriptEnd);
        NEWCMD("SCRIPT",OnScriptLine,All);
        NEWCMD("FORCEQUITBATTLE",OnKickedFromBattle);
        NEWCMD("BROADCAST",OnServerBroadcast,All);
        NEWCMD("SERVERMSGBOX",OnServerMessageBox,All);
        NEWCMD("REDIRECT",OnRedirect,Word,Int);
        NEWCMD("MUTELISTBEGIN",OnMutelistBegin,All);
        NEWCMD("MUTELIST",OnMutelistItem,Word,All);
        NEWCMD("MUTELISTEND",OnMutelistEnd);
        NEWCMD("OFFERFILE",OnFileDownload,Int,Sentence,Sentence,All);
    }

    std::map<std::string,boost::shared_ptr<Commands::CommandBase> > cmd_map_;
};

#undef NEWCMD

} //namespace LSL
#endif // ISERVER_PRIVATE_H
