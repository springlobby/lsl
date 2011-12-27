#ifndef LSL_FUNCTION_TEMPLATES_H
#define LSL_FUNCTION_TEMPLATES_H

/* BEWARE, this file is include _inside_ namespaces in commands.h */

template < class T1 , class T2 , class T3 , class T4, class T5 >
struct Signature {
    typedef boost::function< void (const typename T1::real_type&,
                                   const typename T2::real_type&,
                                   const typename T3::real_type&,
                                   const typename T4::real_type&,
                                   const typename T5::real_type&) > Type; //call traits ftw
    template < class F, class T >
    static Type make( F f, T* x )
    {
        return Type( boost::bind( f, x, _1, _2, _3, _4, _5 ) );
    }
    static void call( Type& func, std::string& params )
    {
        BOOST_AUTO(t1,T1( params )());
        BOOST_AUTO(t2,T2( params )());
        BOOST_AUTO(t3,T3( params )());
        BOOST_AUTO(t4,T4( params )());
        BOOST_AUTO(t5,T5( params )());
        func( t1, t2, t3, t4, t5 );
    }
};

template < class T1 , class T2 , class T3 , class T4 >
struct Signature< T1,T2,T3,T4, Tokens::none > {
    typedef boost::function< void (const typename T1::real_type&,
                                   const typename T2::real_type&,
                                   const typename T3::real_type&,
                                   const typename T4::real_type&) > Type; //call traits ftw
    template < class F, class T >
    static Type make( F f, T* x )
    {
        return Type( boost::bind( f, x, _1, _2, _3, _4 ) );
    }

    static void call( Type& func, std::string& params )
    {
        BOOST_AUTO(t1,T1( params )());
        BOOST_AUTO(t2,T2( params )());
        BOOST_AUTO(t3,T3( params )());
        BOOST_AUTO(t4,T4( params )());

        func( t1, t2, t3, t4 );
    }
};

#endif // LSL_FUNCTION_TEMPLATES_H
