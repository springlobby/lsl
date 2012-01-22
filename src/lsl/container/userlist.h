#ifndef LIBSPRINGLOBBY_HEADERGUARD_USERLIST_H
#define LIBSPRINGLOBBY_HEADERGUARD_USERLIST_H

/** \file misc.h
    \copyright GPL v2 **/

#include "base.h"
#include <lsl/user/user.h>

namespace LSL {

//! contaier for user pointers
class UserList : public ContainerBase< User >
{
public:
    const ConstUserPtr FindByNick( const std::string& nick ) const;
    const UserPtr FindByNick( const std::string& nick );
};

} // namespace LSL

#endif // LIBSPRINGLOBBY_HEADERGUARD_USERLIST_H

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

