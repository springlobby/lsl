#ifndef SPRINGLOBBY_HEADERGUARD_DEBUG_H
#define SPRINGLOBBY_HEADERGUARD_DEBUG_H

#include <stdexcept>
#include "logging.h"

namespace LSL {
namespace Exceptions {

struct base : public std::runtime_error {
	base(const std::string& msg) : std::runtime_error(msg) {}
};
struct file_not_writable: public base {
   file_not_writable(const std::string& msg) : base(msg) {}
};
struct file_not_found : public base {
   file_not_found(const std::string& msg) : base(msg) {}
};
struct unitsync : public base {
   unitsync(const std::string& msg) : base("UNITSYNC: " + msg) {}
};
struct conversion : public base {
   conversion(const std::string& msg) : base("conversion failed: " + msg) {}
};
struct battle : public base {
	template < class BattleType >
   battle(const std::string& msg) : base( "battle exception: " + msg) {}
};
struct function_missing : public unitsync {
   function_missing(const std::string& funcname) : unitsync(" function couldn't be imported: " + funcname) {}
};
} // namespace Exceptions
} // namespace LSL


#define LSL_THROW(excp,msg) do { LslDebug( "%s -- %d", __FILE__,__LINE__); throw LSL::Exceptions::excp(msg);} while(0)

#endif // SPRINGLOBBY_HEADERGUARD_DEBUG_H

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
