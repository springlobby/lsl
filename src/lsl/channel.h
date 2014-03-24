#ifndef LSL_CHANNEL_H
#define LSL_CHANNEL_H

#include <lslutils/global_interfaces.h>
#include <lslutils/type_forwards.h>

namespace LSL {

//! minimal channel model
class Channel : public HasKey< std::string >
{
public:
    Channel();
    Channel(const std::string& name);

    //! the HasKey mandated identifier, must be unique
    std::string key() const { return Name(); }
    static std::string className() { return "Channel"; }

	std::string Name() const { return "dummy"; }

    void OnChannelJoin( const ConstUserPtr user );

    void SetNumUsers( size_t numusers );
	size_t GetNumUsers() const;
    void SetTopic( const std::string& topic);

private:
    std::string m_topic;
};

} // namespace LSL {

/**
 * \file channel.h
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
#endif // LSL_CHANNEL_H
