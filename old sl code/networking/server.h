#ifndef SPRINGLOBBY_HEADERGUARD_SERVER_H
#define SPRINGLOBBY_HEADERGUARD_SERVER_H

#include <wx/string.h>
#include <wx/arrstr.h>

#include "channel/channellist.h"
#include "userlist.h"
#include "battlelist.h"
#include "inetclass.h"
#include "utils/mixins.hh"

class ServerEvents;
class SimpleServerEvents;
class Channel;
class Ui;
struct BattleOptions;
class User;
struct UserBattleStatus;
class ChatPanel;
class wxString;
typedef int ServerError;
class wxColour;


typedef int HostInfo;

#endif // SPRINGLOBBY_HEADERGUARD_SERVER_H
