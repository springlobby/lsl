#ifndef LSL_BATTLE_SIGNALS_H
#define LSL_BATTLE_SIGNALS_H

#include <lslutils/type_forwards.h>
#include <boost/signals2/signal.hpp>

namespace LSL { namespace Signals {

//! battle that was left | User that left | user is a bot
boost::signals2::signal<void (ConstIBattlePtr, ConstUserPtr, bool)> sig_UserLeftBattle;
//! battle that updated | Tag that updated, or empty string
boost::signals2::signal<void (ConstIBattlePtr, std::string)> sig_BattleInfoUpdate;
//! Hosted battle that is ready to start
boost::signals2::signal<void (ConstIBattlePtr)> sig_BattleCouldStartHosted;
//! battle preset stuff has changed
boost::signals2::signal<void ()> sig_ReloadPresetList;


} } // namespace LSL { namespace Signals {

#endif // LSL_BATTLE_SIGNALS_H
