#ifndef LSL_FUNCTION_PTR_H
#define LSL_FUNCTION_PTR_H

#include <boost/extension/shared_library.hpp>
#include <string>
#include <lslutils/debug.h>

namespace LSL {

//!here's some magic that helps us avoid lots of boilerplate for getting pointers
template < class FunctionPointerType, int argument_count >
struct LibFunc{};//if this gets instantiated you need to add more specializations :P

//! LibFunc specialization for 0 arguments
template < class F> struct LibFunc<F,0> {
    static F get( const std::string& name, boost::extensions::shared_library* lib )
    { return lib->get<typename F::result_type>(name); }
};
//! LibFunc specialization for 1 argument
template < class F> struct LibFunc<F,1> {
    static F get( const std::string& name, boost::extensions::shared_library* lib )
    { return lib->get<typename F::result_type,typename F::arg1_type>(name); }
};
//! LibFunc specialization for 2 arguments
template < class F> struct LibFunc<F,2> {
    static F get( const std::string& name, boost::extensions::shared_library* lib )
    { return lib->get<typename F::result_type,typename F::arg1_type,
                typename F::arg2_type>(name); }
};
//! LibFunc specialization for 3 arguments
template < class F> struct LibFunc<F,3> {
    static F get( const std::string& name, boost::extensions::shared_library* lib )
    { return lib->get<typename F::result_type,typename F::arg1_type,
                typename F::arg2_type, typename F::arg3_type>(name); }
};
//! LibFunc specialization for 4 arguments
template < class F> struct LibFunc<F,4> {
    static F get( const std::string& name, boost::extensions::shared_library* lib )
    { return lib->get<typename F::result_type,typename F::arg1_type,
                typename F::arg2_type, typename F::arg3_type,
                typename F::arg4_type>(name); }
};
//! LibFunc specialization for 5 arguments
template < class F> struct LibFunc<F,5> {
    static F get( const std::string& name, boost::extensions::shared_library* lib )
    { return lib->get<typename F::result_type,typename F::arg1_type,
                typename F::arg2_type, typename F::arg3_type,
                typename F::arg4_type,typename F::arg5_type>(name); }
};

//! common loading point for functions from a given library handle
template < class FunctionPointerType >
void GetLibFuncPtr( boost::extensions::shared_library* libhandle, const std::string& name, FunctionPointerType& p )
{
    if (libhandle == NULL || !libhandle->is_open())
      throw std::runtime_error("libhandle not open");
    p = LibFunc<FunctionPointerType,FunctionPointerType::arity>::get( name, libhandle );
    if ( !p ) {
        LslError( "Couldn't load %s from unitsync library",name.c_str() );
    }
}

} // namespace LSL

#endif // LSL_FUNCTION_PTR_H
