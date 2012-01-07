#ifndef ISERVER_PRIVATE_H
#define ISERVER_PRIVATE_H

#include <string>
#include <sstream>
#include <map>
#include <boost/tuple/tuple.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <lslutils/conversion.h>
#include "tasserver.h"

namespace BT = boost::tuples;

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
//struct NoToken{
//	template < class T > NoToken( T& ){}
//};

#define NoToken boost::tuples::null_type

} //namespace Tokens
} //namespace LSL


#include "function_templates.h"

namespace LSL {

struct CommandBase {
	virtual void process( std::string& /*params*/ )
	{
		assert( false ); //means we've called a non-mapped command
	}
};

template < class T0 = NoToken, class T1 = NoToken, class T2 = NoToken, class T3 = NoToken, class T4 = NoToken,
		   class T5 = NoToken, class T6 = NoToken, class T7 = NoToken, class T8 = NoToken, class T9 = NoToken>
struct Command : public CommandBase  {
	typedef boost::tuples::tuple<T0,T1,T2,T3,T4,T5,
				T6,T7,T8,T9> mtuple;
	typedef Signature<mtuple,boost::tuples::length<mtuple>::value>
		SignatureType;
	typedef typename SignatureType::Type
		CallBackType;
	CallBackType func;
	template < class F, class X >
	Command( F f, X* x)
		:func ( SignatureType::make( f, x ) )
	{}
	virtual void process( std::string& params )
	{
		SignatureType::call( func, params );
	}
};

class CommandDictionary {
private:
	//! only TASSERVER can construct a CommandDictionary
	friend class TASServer;
	CommandDictionary( TASServer* tas );

	TASServer* m_tas;
	typedef std::map<std::string,boost::shared_ptr<CommandBase> >
		MapType;
	MapType cmd_map_;

public:
	void Process(const std::string& cmd, std::string& params ) const;
};

} //namespace LSL
#endif // ISERVER_PRIVATE_H
