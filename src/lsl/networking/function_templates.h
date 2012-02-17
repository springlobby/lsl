#ifndef LSL_FUNCTION_TEMPLATES_H
#define LSL_FUNCTION_TEMPLATES_H

/* BEWARE, this file is included in the middle of commands.h */

#define PARSED_VAR(idx) BOOST_AUTO(t ## idx,  (typename boost::tuples::element<idx,Tuple>::type( params )() ) )

namespace LSL {

/** This is the generic Signature with no specialization. No instances of it can
 * be created. If you're trying to you're hitting the current max tuple length.
 * Given a tuple of LSL::Tokens::Basic derivatives this provides a factory method
 * to get a boost::function with each Token's real_type as call args. The "call"
 * function provides a way fitting boost::function with automagically
 * parsing an input string through the given Token Tuple.
 * \tparam Tuple
 * \note since the engine won't depend on availability of c++0x we're limited to 9 args for now
 **/
template < class Tuple, int p_count >
struct Signature{};

//! specialization for 9 Tokens
template < class Tuple >
struct Signature<Tuple,9> {
	typedef boost::function< void (const typename BT::element<0,Tuple>::type::real_type&,
								   const typename BT::element<1,Tuple>::type::real_type&,
								   const typename BT::element<2,Tuple>::type::real_type&,
								   const typename BT::element<3,Tuple>::type::real_type&,
								   const typename BT::element<4,Tuple>::type::real_type&,
								   const typename BT::element<5,Tuple>::type::real_type&,
								   const typename BT::element<6,Tuple>::type::real_type&,
								   const typename BT::element<7,Tuple>::type::real_type&,
								   const typename BT::element<8,Tuple>::type::real_type&) > Type; //call traits ftw
	template < class F, class T >
	static Type make( F f, T* x )
	{
		return Type( boost::bind( f, x, _1, _2, _3, _4, _5, _6, _7, _8, _9 ) );
	}
	static void call( Type& func, std::string& params )
	{
		PARSED_VAR(0);
		PARSED_VAR(1);PARSED_VAR(2);
		PARSED_VAR(3);PARSED_VAR(4);
		PARSED_VAR(5);PARSED_VAR(6);
		PARSED_VAR(7);PARSED_VAR(8);
		func( t0, t1, t2, t3, t4, t5, t6, t7, t8 );
	}
};

//! specialization for 8 Tokens
template < class Tuple >
struct Signature<Tuple,8> {
	typedef boost::function< void (const typename BT::element<0,Tuple>::type::real_type&,
								   const typename BT::element<1,Tuple>::type::real_type&,
								   const typename BT::element<2,Tuple>::type::real_type&,
								   const typename BT::element<3,Tuple>::type::real_type&,
								   const typename BT::element<4,Tuple>::type::real_type&,
								   const typename BT::element<5,Tuple>::type::real_type&,
								   const typename BT::element<6,Tuple>::type::real_type&,
								   const typename BT::element<7,Tuple>::type::real_type&) > Type; //call traits ftw
	template < class F, class T >
	static Type make( F f, T* x )
	{
		return Type( boost::bind( f, x, _1, _2, _3, _4, _5, _6, _7, _8 ) );
	}
	static void call( Type& func, std::string& params )
	{
		PARSED_VAR(0);
		PARSED_VAR(1);PARSED_VAR(2);
		PARSED_VAR(3);PARSED_VAR(4);
		PARSED_VAR(5);PARSED_VAR(6);
		PARSED_VAR(7);
		func( t0, t1, t2, t3, t4, t5, t6, t7 );
	}
};

//! specialization for 7 Tokens
template < class Tuple >
struct Signature<Tuple,7> {
	typedef boost::function< void (const typename BT::element<0,Tuple>::type::real_type&,
								   const typename BT::element<1,Tuple>::type::real_type&,
								   const typename BT::element<2,Tuple>::type::real_type&,
								   const typename BT::element<3,Tuple>::type::real_type&,
								   const typename BT::element<4,Tuple>::type::real_type&,
								   const typename BT::element<5,Tuple>::type::real_type&,
								   const typename BT::element<6,Tuple>::type::real_type&) > Type; //call traits ftw
	template < class F, class T >
	static Type make( F f, T* x )
	{
		return Type( boost::bind( f, x, _1, _2, _3, _4, _5, _6, _7 ) );
	}
	static void call( Type& func, std::string& params )
	{
		PARSED_VAR(0);
		PARSED_VAR(1);PARSED_VAR(2);
		PARSED_VAR(3);PARSED_VAR(4);
		PARSED_VAR(5);PARSED_VAR(6);
		func( t0, t1, t2, t3, t4, t5, t6 );
	}
};

//! specialization for 6 Tokens
template < class Tuple >
struct Signature<Tuple,6> {
	typedef boost::function< void (const typename BT::element<0,Tuple>::type::real_type&,
								   const typename BT::element<1,Tuple>::type::real_type&,
								   const typename BT::element<2,Tuple>::type::real_type&,
								   const typename BT::element<3,Tuple>::type::real_type&,
								   const typename BT::element<4,Tuple>::type::real_type&,
								   const typename BT::element<5,Tuple>::type::real_type&) > Type; //call traits ftw
	template < class F, class T >
	static Type make( F f, T* x )
	{
		return Type( boost::bind( f, x, _1, _2, _3, _4, _5, _6 ) );
	}
	static void call( Type& func, std::string& params )
	{
		PARSED_VAR(0);
		PARSED_VAR(1);PARSED_VAR(2);
		PARSED_VAR(3);PARSED_VAR(4);
		PARSED_VAR(5);
		func( t0, t1, t2, t3, t4, t5 );
	}
};

//! specialization for 5 Tokens
template < class Tuple >
struct Signature<Tuple,5> {
	typedef boost::function< void (const typename BT::element<0,Tuple>::type::real_type&,
								   const typename BT::element<1,Tuple>::type::real_type&,
								   const typename BT::element<2,Tuple>::type::real_type&,
								   const typename BT::element<3,Tuple>::type::real_type&,
								   const typename BT::element<4,Tuple>::type::real_type&) > Type; //call traits ftw
    template < class F, class T >
    static Type make( F f, T* x )
    {
        return Type( boost::bind( f, x, _1, _2, _3, _4, _5 ) );
    }
    static void call( Type& func, std::string& params )
    {
		PARSED_VAR(0);
		PARSED_VAR(1);PARSED_VAR(2);
		PARSED_VAR(3);PARSED_VAR(4);
		func( t0, t1, t2, t3, t4 );
    }
};

//! specialization for 4 Tokens
template < class Tuple >
struct Signature<Tuple,4> {
	typedef boost::function< void (const typename BT::element<0,Tuple>::type::real_type&,
								   const typename BT::element<1,Tuple>::type::real_type&,
								   const typename BT::element<2,Tuple>::type::real_type&,
								   const typename BT::element<3,Tuple>::type::real_type&) > Type; //call traits ftw
    template < class F, class T >
    static Type make( F f, T* x )
    {
        return Type( boost::bind( f, x, _1, _2, _3, _4 ) );
    }

    static void call( Type& func, std::string& params )
    {
		PARSED_VAR(0);PARSED_VAR(1);
		PARSED_VAR(2);PARSED_VAR(3);
		func( t0, t1, t2, t3 );
    }
};

//! specialization for 3 Tokens
template < class Tuple >
struct Signature<Tuple,3> {
	typedef boost::function< void (const typename BT::element<0,Tuple>::type::real_type&,
								   const typename BT::element<1,Tuple>::type::real_type&,
								   const typename BT::element<2,Tuple>::type::real_type&) > Type; //call traits ftw
	template < class F, class T >
	static Type make( F f, T* x )
	{
		return Type( boost::bind( f, x, _1, _2, _3 ) );
	}

	static void call( Type& func, std::string& params )
	{
		PARSED_VAR(0);PARSED_VAR(1);
		PARSED_VAR(2);
		func( t0, t1, t2 );
	}
};

//! specialization for 2 Tokens
template < class Tuple >
struct Signature<Tuple,2> {
	typedef boost::function< void (const typename BT::element<0,Tuple>::type::real_type&,
								   const typename BT::element<1,Tuple>::type::real_type&) > Type; //call traits ftw
	template < class F, class T >
	static Type make( F f, T* x )
	{
		return Type( boost::bind( f, x, _1, _2 ) );
	}

	static void call( Type& func, std::string& params )
	{
		PARSED_VAR(0);PARSED_VAR(1);
		func( t0, t1 );
	}
};

//! specialization for 1 Token
template < class Tuple >
struct Signature<Tuple,1> {
	typedef boost::function< void (const typename BT::element<0,Tuple>::type::real_type&) > Type; //call traits ftw
	template < class F, class T >
	static Type make( F f, T* x )
	{
		return Type( boost::bind( f, x, _1 ) );
	}

	static void call( Type& func, std::string& params )
	{
		PARSED_VAR(0);
		func( t0 );
	}
};

//! specialization for no Tokens
template < class Tuple >
struct Signature<Tuple,0> {
	typedef boost::function< void () > Type; //call traits ftw
	template < class F, class T >
	static Type make( F f, T* x )
	{
		return Type( boost::bind( f, x ) );
	}

	static void call( Type& func, std::string& params )
	{
		func(  );
	}
};


} // namespace LSL {

/**
 * \file function_templates.h
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

#endif // LSL_FUNCTION_TEMPLATES_H
