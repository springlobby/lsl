#ifndef ISERVER_PRIVATE_H
#define ISERVER_PRIVATE_H

#include <string>
#include <sstream>
#include <boost/tuple/tuple.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/function.hpp>

#include <utils/conversion.h>
#include "tasserver.h"

namespace LSL {

std::string GetParamByChar( std::string& params, const char sep )
{
	std::string::size_type pos = params.find( sep );
	std::string ret;
	if ( pos != -1 )
	{
		ret = params.substr( 0,pos );//inclusive??
		params = params.substr( pos + 1 );
	}
	else
	{
		ret = params;
		params = "";
	}
	return ret;
}

std::string GetWordParam( std::string& params )
{
	return GetParamByChar( params, ' ');
}

std::string GetSentenceParam( std::string& params )
{
	return GetParamByChar( params, '\t' );
}

long GetIntParam( std::string& params )
{
	const std::string d = GetParamByChar( params, ' ');
	return Util::FromString<long>( d );
}

bool GetBoolParam( std::string& params )
{
	return (bool)GetIntParam( params );
}

namespace Tokens {

template < class TypeImp >
struct Basic {
	typedef TypeImp
		real_type;
	const real_type value;

	Basic( real_type v )
		:value(v)
	{}

	real_type operator ()() const
	{ return value; }
};
struct Word : public Basic<std::string>{
	Word( std::string& params )
		:Basic<std::string>( GetWordParam( params ) ){}
};
struct Sentence : public Basic<std::string> {
	Sentence( std::string& params )
		:Basic<std::string>( GetWordParam( params ) ){}
} ;
struct Int : public Basic<int>{
	Int( std::string& params )
		:Basic<int>( GetIntParam( params ) ){}
} ;
struct Float: public Basic<float>{
	Float( std::string& params )
		:Basic<float>( GetIntParam( params ) ){}
} ;
struct Double: public Basic<double>{
	Double( std::string& params )
		:Basic<double>( GetIntParam( params ) ){}
} ;
//! this effectively ends further parsing by consuming all params
struct All: public Basic<std::string>{
	All( std::string& params )
		:Basic<std::string>( params )
	{ params = ""; }
} ;
struct none{
	template < class T > none( T& ){}
};

} //namespace Tokens

namespace Commands {

#include "function_templates.h"

struct CommandBase {
	virtual void process( std::string& params )
	{
		assert( false ); //means we've called a non-mapped command
	}
};

template < class T1 = Tokens::none, class T2 = Tokens::none, class T3 = Tokens::none, class T4 = Tokens::none, class T5 = Tokens::none >
struct Command : public CommandBase  {
	typedef boost::tuples::tuple<T1,T2,T3,T4,T5> mtuple;
	typedef Signature<T1,T2,T3,T4,T5> SignatureType;
	typedef typename SignatureType::Type CallBackType;
	CallBackType func;
	template < class F, class T >
	Command( F f, T* x)
		:func ( SignatureType::make( f, x ) )
	{}
	virtual void process( std::string& params )
	{
		SignatureType::call( func, params );
	}
};

} //namespace Commands


class CommandDictionary {
private:
	//! only TASSERVER can construct a CommandDictionary
	friend class TASServer;
	CommandDictionary( TASServer* tas );

	TASServer* m_tas;
	std::map<std::string,boost::shared_ptr<Commands::CommandBase> > cmd_map_;
};

} //namespace LSL
#endif // ISERVER_PRIVATE_H
