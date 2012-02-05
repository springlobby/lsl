#ifndef SPRINGLOBBY_HEADERGUARD_SPRING_H
#define SPRINGLOBBY_HEADERGUARD_SPRING_H

#include <lslutils/type_forwards.h>
#include <boost/signals2.hpp>

namespace LSL {
namespace Battle {
class OfflineBattle;
}
class SpringProcess;

class Spring
{
  public:
	explicit Spring();
    ~Spring();

    bool IsRunning() const;
    /**
     * @brief executes spring with abs path to script
     * @param script
     * @return true on launch success, false otherwise
     */
    bool Run( const std::string& script );
    bool Run( const BattlePtr battle );
    bool Run( Battle::OfflineBattle& battle );

    /** @brief executes spring with replay abs path
     * @param filename the full path for the replayfile
     * @return true on launch success, false otherwise
     **/
    bool RunReplay ( const std::string& filename );

    std::string WriteScriptTxt( const ConstIBattlePtr battle ) const;
    void OnTerminated( int event );

    boost::signals2::signal<void (int,std::string)> sig_springStopped;
    boost::signals2::signal<void ()> sig_springStarted;

protected:
    bool LaunchSpring( const std::string& params );

    SpringProcess* m_process;
    bool m_running;
};

Spring& spring();

} // namespace LSL {

#endif // SPRINGLOBBY_HEADERGUARD_SPRING_H

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

