#ifndef OFFLINEBATTLE_H_INCLUDED
#define OFFLINEBATTLE_H_INCLUDED

#include "ibattle.h"

namespace LSL {
namespace Battle {

class OfflineBattle : public IBattle
{
	public:
			OfflineBattle ( const int id );
			OfflineBattle ( );
			OfflineBattle ( const OfflineBattle&  );
			OfflineBattle& operator = ( const OfflineBattle&  );
			~OfflineBattle (){}
            virtual const UserPtr GetMe() { return m_me; }
            virtual const ConstUserPtr GetMe() const { return m_me; }
			bool IsFounderMe() const { return true; }
			void StartSpring();

	protected:
			int m_id;
            UserPtr m_me;
};

} // namespace Battle {
} // namespace LSL {

/**
 * \file offline.h
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

#endif // OFFLINEBATTLE_H_INCLUDED
