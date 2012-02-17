#ifndef LSL_HEADERGUARD_SINGLEPLAYERBATTLE_H
#define LSL_HEADERGUARD_SINGLEPLAYERBATTLE_H

#include "ibattle.h"
#include <lsl/user/user.h>

namespace LSL {
namespace Battle {

class SinglePlayerBattle: public IBattle
{
  public:

    SinglePlayerBattle(  );
    virtual ~SinglePlayerBattle();

// (koshi) these are never called
//    unsigned int AddBot( int ally, int posx, int posy, int handicap, const std::string& aidll );
//    void UpdateBot( unsigned int index, int ally, int posx, int posy, int side );

	bool IsFounderMe() const { return true; }

    virtual const UserPtr GetMe() { return m_me; }
    virtual const ConstUserPtr GetMe() const { return m_me; }

    void SendHostInfo( Enum::HostInfo update );
    void SendHostInfo( const std::string& /*unused*/ ){ }

    void Update( const std::string& Tag );

    void StartSpring();

  protected:
    void RemoveUnfittingBots();

    UserPtr m_me;
};

} // namespace Battle {
} // namespace LSL {

/**
 * \file singleplayer.h
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

#endif // LSL_HEADERGUARD_SINGLEPLAYERBATTLE_H
