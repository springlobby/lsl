#ifndef LSL_GLOBALSMANAGER_H
#define LSL_GLOBALSMANAGER_H

#include <stdexcept>
#include <assert.h>
#include "lslutils/conversion.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

namespace LSL {
namespace Util {

template < class T >
struct LineInfo {
    LineInfo(const char* at )
        : m( at )
    {}
    std::string  m;
};

class GlobalDestroyedError: public std::runtime_error
{
public:
    GlobalDestroyedError(): std::runtime_error( "trying to access global during or after DestroyAll" )
    {

    }
};

class GlobalRecursiveError: public std::runtime_error
{
	public:
			GlobalRecursiveError(): std::runtime_error( "trying to access global during its construction" )
			{

			}
};

class IGlobalObjectHolder
{
	public:
		virtual ~IGlobalObjectHolder(){}
		bool RegisterSelf();
		virtual void Nullify() = 0;
		virtual void Destroy() = 0;
};

void DestroyGlobals();

template<class T, class I >
class GlobalObjectHolder: public IGlobalObjectHolder
{
    T *private_ptr;
    T *public_ptr;
    bool constructing;
    static int count;

public:
	GlobalObjectHolder(I )
		: private_ptr( NULL )
		, public_ptr( NULL )
		, constructing( true )
    {
        GlobalObjectHolder<T,I>::count += 1;
        assert( (GlobalObjectHolder<T,I>::count) == 1 );
        if ( RegisterSelf() )
        {
            private_ptr = new T;
            public_ptr = private_ptr;
        }
        constructing = false;
    }
	~GlobalObjectHolder()
	{
		Destroy();
	}

    virtual void Nullify()
    {
        public_ptr = NULL;
	}
    virtual void Destroy()
    {
        public_ptr = NULL;
        delete private_ptr;
        private_ptr = NULL;
	}
    T &GetInstance()
    {
        if ( constructing ) throw GlobalRecursiveError();
        if ( !public_ptr ) throw GlobalDestroyedError();
        return *public_ptr;
    }
    operator T&()
    {
        return GetInstance();
    }
	const T &GetInstance() const
	{
		if ( constructing ) throw GlobalRecursiveError();
		if ( !public_ptr ) throw GlobalDestroyedError();
		return *public_ptr;
	}
	operator const T&() const
	{
		return GetInstance();
	}
};
template<class T, class I >
int GlobalObjectHolder<T,I>::count = 0;

} // namespace LSL {
} // namespace Util {
/**
 * \file globalsmanager.h
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

#endif // LSL_GLOBALSMANAGER_H
