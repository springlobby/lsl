#ifndef LSL_HEADERGUARD_BATTLE_SIGNALS_H
#define LSL_HEADERGUARD_BATTLE_SIGNALS_H

#include <lslutils/type_forwards.h>
#include <boost/signals2/signal.hpp>

namespace LSL { namespace Signals {

/** \addtogroup signals 
 *  @{
 */
//! battle that was left | User that left | user is a bot
static boost::signals2::signal<void (ConstIBattlePtr, ConstUserPtr, bool)> sig_UserLeftBattle;
//! battle that updated | Tag that updated, or empty string
static boost::signals2::signal<void (ConstIBattlePtr, std::string)> sig_BattleInfoUpdate;
//! Hosted battle that is ready to start
static boost::signals2::signal<void (ConstIBattlePtr)> sig_BattleCouldStartHosted;
//! battle preset stuff has changed
static boost::signals2::signal<void ()> sig_ReloadPresetList;
/** @}*/

} } // namespace LSL { namespace Signals {

/**
 * \file signals.h
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

#endif // LSL_HEADERGUARD_BATTLE_SIGNALS_H
