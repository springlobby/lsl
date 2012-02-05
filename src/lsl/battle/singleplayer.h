#ifndef SPRINGLOBBY_HEADERGUARD_SINGLEPLAYERBATTLE_H
#define SPRINGLOBBY_HEADERGUARD_SINGLEPLAYERBATTLE_H

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

#endif // SPRINGLOBBY_HEADERGUARD_SINGLEPLAYERBATTLE_H

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

