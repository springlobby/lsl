#ifndef LSL_GLOBALSMANAGER_H
#define LSL_GLOBALSMANAGER_H

/** \file globalsmanager.h
    \copyright GPL v2 **/

#include <stdexcept>
#include <assert.h>
#include "lslutils/conversion.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

namespace LSL {

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

#endif // LSL_GLOBALSMANAGER_H

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

