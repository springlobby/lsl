#ifndef LSL_BATTLE_SIGNALS_H
#define LSL_BATTLE_SIGNALS_H

#include <utils/type_forwards.h>
#include <boost/signals2/signal.hpp>

namespace LSL { namespace Signals {

//! battle that was left | User that left | user is a bot
boost::signals2::signal<void (ConstBattlePtr, ConstUserPtr, bool)> sig_UserLeftBattle;
//! battle that updated | Tag that updated, or empty string
boost::signals2::signal<void (ConstBattlePtr, std::string)> sig_BattleInfoUpdate;
//! Hosted battle that is ready to start
boost::signals2::signal<void (ConstBattlePtr)> sig_BattleCouldStartHosted;


} } // namespace LSL { namespace Signals {

#endif // LSL_BATTLE_SIGNALS_H
