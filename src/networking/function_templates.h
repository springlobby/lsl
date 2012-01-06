#ifndef LSL_FUNCTION_TEMPLATES_H
#define LSL_FUNCTION_TEMPLATES_H

/* BEWARE, this file is included in commands.h */

#define PARSED_VAR(idx) BOOST_AUTO(t ## idx,  (typename boost::tuples::element<idx,Tuple>::type( params )() ) )

namespace LSL {

//! since the engine won't depend on availability of c++0x and i'm too lazy to figure out tr1 tuple checks, we're limiting to 9 args for now
template < class Tuple, int p_count >
struct Signature{};

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

#endif // LSL_FUNCTION_TEMPLATES_H
