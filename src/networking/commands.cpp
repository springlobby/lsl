#include "commands.h"

#define NEWCMD(Name,Func,T1,T2,T3,T4) \
    cmd_map_[Name] = boost::shared_ptr<Commands::Command<Tokens::T1,Tokens::T2,Tokens::T3,Tokens::T4> > \
    ( new Commands::Command<Tokens::T1,Tokens::T2,Tokens::T3,Tokens::T4>(&TASServer::Func, m_tas) )

CommandDictionary::CommandDictionary( TASServer* tas )
    :m_tas(tas)
{
//        NEWCMD("ADDUSER",OnNewUser,Word,Word,Int,Int);
//        NEWCMD("TASSERVER",OnConnected,Word,Int,Word,Int);
//        NEWCMD("ACCEPTED",OnLogin,Word);
//        NEWCMD("MOTD",OnMotd,All);
    NEWCMD("ADDUSER",OnNewUser,Word,Word,Int,Int);
//        NEWCMD("CLIENTSTATUS",OnUserStatusChanged,Word,Int);
//        NEWCMD("BATTLEOPENED",OnBattleOpened,Int,Int,Int,Word,Word,Int,Int,Int,Int,Word,Sentence,Sentence,Sentence,Sentence);
//        NEWCMD("JOINEDBATTLE",OnUserJoinedBattle,Int,Word,Word);
//        NEWCMD("UPDATEBATTLEINFO",OnBattleInfoUpdated,Int,Int,Int,Word,Sentence);
//        NEWCMD("LOGININFOEND",OnLoginInfoComplete);
//        NEWCMD("REMOVEUSER",OnUserQuit,Word);
//        NEWCMD("BATTLECLOSED",OnBattleClosed,Int);
//        NEWCMD("LEFTBATTLE",OnUserLeftBattle,Int,Word);
//        NEWCMD("JOIN",OnJoinChannel,Word,Sentence);
//        NEWCMD("SAID",OnChannelSaid,Word,Word,All);
//        NEWCMD("JOINED",OnChannelSaid,Word,Word);
//        NEWCMD("LEFT",OnChannelPart,Word,Word,All);
//        NEWCMD("CHANNELTOPIC",OnChannelTopic,Word,Word,Int,All);
//        NEWCMD("SAIDEX",OnChannelAction,Word,Word,All);
//        NEWCMD("CLIENTS",OnChannelJoinUserList,Word,All);
//        NEWCMD("SAYPRIVATE",OnPrivateMessage,Word,All);
//        NEWCMD("SAYPRIVATEEX",OnSayPrivateMessageEx,Word,All);
//        NEWCMD("SAIDPRIVATEEX",OnSaidPrivateMessageEx,Word,All);
//        NEWCMD("JOINBATTLE",OnJoinedBattle,Int,Word);
//        NEWCMD("CLIENTBATTLESTATUS",OnClientBattleStatus,Word,Int,Int);
//        NEWCMD("ADDSTARTRECT",OnBattleStartRectAdd,Int,Int,Int,Int,Int);
//        NEWCMD("REMOVESTARTRECT",OnBattleStartRectRemove,Int);
//        NEWCMD("ENABLEALLUNITS",OnBattleEnableAllUnits);
//        NEWCMD("ENABLEUNITS",OnBattleEnableUnits,All);
//        NEWCMD("DISABLEUNITS",OnBattleDisableUnits,All);
//        NEWCMD("CHANNEL",OnChannelListEntry,Word,Int,All);
//        NEWCMD("ENDOFCHANNELS",OnChannelListEnd);
//        NEWCMD("REQUESTBATTLESTATUS",OnRequestBattleStatus);
//        NEWCMD("SAIDBATTLE",OnSaidBattle,Word,All);
//        NEWCMD("SAIDBATTLEEX",OnBattleAction,Word,All);
//        NEWCMD("AGREEMENT",OnAgreenmentLine,All);
//        NEWCMD("AGREEMENTEND",OnAcceptAgreement);
//        NEWCMD("OPENBATTLE",OnHostedBattle,Int);
//        NEWCMD("ADDBOT",OnBattleAddBot,Int,Word,Word,Int,Int,All);
//        NEWCMD("UPDATEBOT",OnBattleUpdateBot,Int,Word,Int,Int);
//        NEWCMD("REMOVEBOT",OnBattleRemoveBot,Int,Word);
//        NEWCMD("RING",OnRing,Word);
//        NEWCMD("SERVERMSG",OnServerMessage,All);
//        NEWCMD("JOINBATTLEFAILED",OnJoinBattleFailed,All);
//        NEWCMD("OPENBATTLEFAILED",OnOpenBattleFailed,All);
//        NEWCMD("JOINFAILED",OnJoinChannelFailed,All);
//        NEWCMD("ACQUIREUSERID",OnGetHandle);
//        NEWCMD("FORCELEAVECHANNEL",OnKickedFromChannel,Word,Word,All);
//        NEWCMD("DENIED",OnLoginFailed,All);
//        NEWCMD("HOSTPORT",OnHostExternalUdpPort,Int);
//        NEWCMD("UDPSOURCEPORT",OnMyExternalUdpSourcePort,Int);
//        NEWCMD("CLIENTIPPORT",OnClientIPPort,Word,Word,Int);
//        NEWCMD("SETSCRIPTTAGS",OnSetBattleInfo,All);
//        NEWCMD("SCRIPTSTART",OnScriptStart);
//        NEWCMD("SCRIPTEND",OnScriptEnd);
//        NEWCMD("SCRIPT",OnScriptLine,All);
//        NEWCMD("FORCEQUITBATTLE",OnKickedFromBattle);
//        NEWCMD("BROADCAST",OnServerBroadcast,All);
//        NEWCMD("SERVERMSGBOX",OnServerMessageBox,All);
//        NEWCMD("REDIRECT",OnRedirect,Word,Int);
//        NEWCMD("MUTELISTBEGIN",OnMutelistBegin,All);
//        NEWCMD("MUTELIST",OnMutelistItem,Word,All);
//        NEWCMD("MUTELISTEND",OnMutelistEnd);
//        NEWCMD("OFFERFILE",OnFileDownload,Int,Sentence,Sentence,All);
}

