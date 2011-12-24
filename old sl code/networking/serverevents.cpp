/* Copyright (C) 2007 The SpringLobby Team. All rights reserved. */
//
// Class: ServerEvents
//

#ifdef _MSC_VER
#ifndef NOMINMAX
	#define NOMINMAX
#endif // NOMINMAX
#include <winsock2.h>
#endif // _MSC_VER

#include <wx/intl.h>
#include <stdexcept>

#include "serverevents.h"
#include "mainwindow.h"
#include "ui.h"
#include "channel/channel.h"
#include "user.h"
#include "utils/debug.h"
#include "uiutils.h"
#include "server.h"
#include "battle.h"
#include "httpdownloader.h"
#include "settings.h"
#include "utils/customdialogs.h"
#include "utils/tasutil.h"
#include "utils/uievents.h"

#ifndef NO_TORRENT_SYSTEM
#include "torrentwrapper.h"
#endif
#include "globalsmanager.h"
