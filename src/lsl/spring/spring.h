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

    std::string WriteScriptTxt(const IBattlePtr battle ) const;
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

/**
 * \file spring.h
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

#endif // SPRINGLOBBY_HEADERGUARD_SPRING_H
