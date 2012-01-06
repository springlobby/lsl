#ifndef LSL_FUNCTION_TEMPLATES_H
#define LSL_FUNCTION_TEMPLATES_H

/* BEWARE, this file is included in commands.h */

#define PARSED_VAR(idx) BOOST_AUTO(t ## idx,  (typename boost::tuples::element<idx,Tuple>::type( params )() ) )

namespace LSL {

//! since the engine won't depend on availability of c++0x and i'm too lazy to figure out tr1 tuple checks, we're limiting to 9 args for now
template < class Tuple, int p_count >
struct Signature{};

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


} // namespace LSL {

#endif // LSL_FUNCTION_TEMPLATES_H
