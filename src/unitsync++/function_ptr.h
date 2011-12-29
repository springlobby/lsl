#ifndef LSL_FUNCTION_PTR_H
#define LSL_FUNCTION_PTR_H

#include <boost/extension/shared_library.hpp>
#include <string>
#include <utils/debug.h>

namespace LSL {

//!here's some magic that helps us avoid lots of boilerplate for getting pointers
template < class FunctionPointerType, int argN >
struct LibFunc{};//if this gets instantiated you need to add more specializations :P

template < class F> struct LibFunc<F,0> {
    static F get( const std::string& name, boost::extensions::shared_library* lib )
    { return lib->get<typename F::result_type>(name); }
};
template < class F> struct LibFunc<F,1> {
    static F get( const std::string& name, boost::extensions::shared_library* lib )
    { return lib->get<typename F::result_type,typename F::arg1_type>(name); }
};
template < class F> struct LibFunc<F,2> {
    static F get( const std::string& name, boost::extensions::shared_library* lib )
    { return lib->get<typename F::result_type,typename F::arg1_type,
                typename F::arg2_type>(name); }
};
template < class F> struct LibFunc<F,3> {
    static F get( const std::string& name, boost::extensions::shared_library* lib )
    { return lib->get<typename F::result_type,typename F::arg1_type,
                typename F::arg2_type, typename F::arg3_type>(name); }
};
template < class F> struct LibFunc<F,4> {
    static F get( const std::string& name, boost::extensions::shared_library* lib )
    { return lib->get<typename F::result_type,typename F::arg1_type,
                typename F::arg2_type, typename F::arg3_type,
                typename F::arg4_type>(name); }
};
template < class F> struct LibFunc<F,5> {
    static F get( const std::string& name, boost::extensions::shared_library* lib )
    { return lib->get<typename F::result_type,typename F::arg1_type,
                typename F::arg2_type, typename F::arg3_type,
                typename F::arg4_type,typename F::arg5_type>(name); }
};

template < class FunctionPointerType >
void GetLibFuncPtr( boost::extensions::shared_library* libhandle, const std::string& name, FunctionPointerType& p )
{
    if ( !libhandle && libhandle->is_open() )
        throw std::runtime_error("libhandle not open");
    p = LibFunc<FunctionPointerType,FunctionPointerType::arity>::get( name, libhandle );
    if ( !p ) {
        LslError( "Couldn't load %s from unitsync library",name.c_str() );
    }
}

} // namespace LSL

#endif // LSL_FUNCTION_PTR_H
