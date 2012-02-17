#ifndef LSL_HEADERGUARD_DEBUG_H
#define LSL_HEADERGUARD_DEBUG_H

#include <stdexcept>
#include <string>
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
   battle(const std::string& msg) : base( "battle exception: " + msg) {}
};
struct server : public base {
   server(const std::string& msg) : base( "server exception: " + msg) {}
};
struct function_missing : public unitsync {
   function_missing(const std::string& funcname) : unitsync(" function couldn't be imported: " + funcname) {}
};
} // namespace Exceptions
} // namespace LSL


#define LSL_THROW(excp,msg) do { LslDebug( "%s -- %d", __FILE__,__LINE__); throw LSL::Exceptions::excp(msg);} while(0)

/**
 * \file debug.h
 * \section LICENSE
Copyright 2012 by The libSpringLobby team. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#endif // LSL_HEADERGUARD_DEBUG_H
