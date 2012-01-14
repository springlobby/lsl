/* Copyright (C) 2007 The SpringLobby Team. All rights reserved. */
#include "userlist.h"

namespace LSL {

const ConstUserPtr UserList::FindByNick( const std::string& nick ) const
{
    MapType::const_iterator it = find( nick );
    if ( it != end() )
        return it->second;
    return ConstUserPtr();
}

const UserPtr UserList::FindByNick(const std::string &nick)
{
    MapType::const_iterator it = find( nick );
    if ( it != end() )
        return it->second;
    return UserPtr();
}

}
