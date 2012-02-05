#ifndef SPRINGLOBBY_HEADERGUARD_SPRINGPROCESS_H
#define SPRINGLOBBY_HEADERGUARD_SPRINGPROCESS_H

#include <string>
#include <lslutils/type_forwards.h>

namespace LSL {

class Spring;

class SpringProcess
{
  public:
    SpringProcess( const Spring& sp );
    ~SpringProcess();

    void OnExit();

    void SetCommand( const std::string& cmd );

    void Create() {}
    int Run();

  protected:
    const Spring& m_sp;
    std::string m_cmd;
    int m_exit_code;
};

} // namespace LSL {

#endif // SPRINGLOBBY_HEADERGUARD_SPRINGPROCESS_H

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

