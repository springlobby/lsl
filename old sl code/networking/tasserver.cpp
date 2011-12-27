/* Copyright (C) 2007-2011 The SpringLobby Team. All rights reserved. */


#ifdef _MSC_VER
#ifndef NOMINMAX
	#define NOMINMAX
#endif // NOMINMAX
#include <winsock2.h>
#endif // _MSC_VER

#include <wx/string.h>
#include <wx/regex.h>
#include <wx/intl.h>
#include <wx/protocol/http.h>
#include <wx/socket.h>
#include <wx/log.h>
#include <wx/tokenzr.h>
#include <wx/platinfo.h>
#include <wx/stopwatch.h>

#include <stdexcept>
#include <algorithm>

#include <map>

#include "base64.h"
#include "utils/md5.h"
#include "tasserver.h"
#include "springunitsync.h"
#include "user.h"
#include "utils/debug.h"
#include "utils/tasutil.h"
#include "utils/conversion.h"
#include "utils/platform.h"
#include "updater/updatehelper.h"
#include "battle.h"
#include "serverevents.h"
#include "socket.h"
#include "channel/channel.h"
#include "tasservertokentable.h"
#include "pingthread.h"

// for SL_MAIN_ICON
#include "utils/customdialogs.h"

#include "settings.h"
