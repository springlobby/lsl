#ifndef SPRINGLOBBY_HEADERGUARD_IBATTLE_H
#define SPRINGLOBBY_HEADERGUARD_IBATTLE_H


#include <wx/string.h>
#include <wx/event.h>

#include "springunitsync.h"
#include "user.h"
#include "mmoptionswrapper.h"
#include "userlist.h"
#include "tdfcontainer.h"
#include "utils/isink.h"
#include "utils/mixins.hh"

const unsigned int DEFAULT_SERVER_PORT = 8452;
const unsigned int DEFAULT_EXTERNAL_UDP_SOURCE_PORT = 16941;

class IBattle;
class wxTimer;

struct BattleStartRect
{
    BattleStartRect() :
        toadd(false),
        todelete(false),
        toresize(false),
        exist(false),
        ally(-1),
        top(-1),
        left(-1),
        right(-1),
        bottom(-1)
    {
    }

    bool toadd;
    bool todelete;
    bool toresize;
    bool exist;

    int ally;
    int top;
    int left;
    int right;
    int bottom;

    bool IsOk() const
    {
        return exist && !todelete;
    }
};


enum NatType
{
		NAT_None = 0,
		NAT_Hole_punching,
		NAT_Fixed_source_ports
};


enum BattleType
{
		BT_Played,
		BT_Replay,
		BT_Savegame
};


struct BattleOptions
{
	BattleOptions() :
		battleid(-1),islocked(false),battletype(BT_Played),ispassworded(false),rankneeded(0),proxyhost(_T("")),userelayhost(false),lockexternalbalancechanges(false),
		nattype(NAT_None),port(DEFAULT_SERVER_PORT),externaludpsourceport(DEFAULT_EXTERNAL_UDP_SOURCE_PORT),internaludpsourceport(DEFAULT_EXTERNAL_UDP_SOURCE_PORT),maxplayers(0),spectators(0),
		guilistactiv(false) {}

	int battleid;
	bool islocked;
	BattleType battletype;
	bool ispassworded;
	int rankneeded;
	wxString proxyhost;
	bool userelayhost;
	bool lockexternalbalancechanges;

	wxString founder;

	NatType nattype;
	unsigned int port;
	wxString ip;
	unsigned int externaludpsourceport;
	unsigned int internaludpsourceport;

	unsigned int maxplayers;
	unsigned int spectators;
	wxString relayhost;
	wxString maphash;
	wxString modhash;

	wxString description;
	wxString mapname;
	wxString modname;

	bool guilistactiv;
};



#endif // SPRINGLOBBY_HEADERGUARD_IBATTLE_H

/**
    This file is part of SpringLobby,
    Copyright (C) 2007-2011

    SpringLobby is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as published by
    the Free Software Foundation.

    SpringLobby is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SpringLobby.  If not, see <http://www.gnu.org/licenses/>.
**/

