//SpringUnitSync& usync();

#include <iostream>
#include <boost/extension/shared_library.hpp>
#include <boost/function.hpp>
//#include <unitsync++/c_api.h>


template < class F, int argN >
struct LibFunc{};
template < class F>
struct LibFunc<F,0>
{
    static F get( const std::string& name, boost::extensions::shared_library* lib )
    { return lib->get<typename F::result_type>(name); }
};

template < class F >
void getMe( const std::string name , boost::extensions::shared_library* lib, F& f )
{
    f = LibFunc<F,F::arity>::get( name, lib );
}

void dummySync()
{
    using namespace boost::extensions;

      // In the Jamfile, shared libraries are set to have the same
      // prefix and extension, even on different operating systems.
      // This is for convenience in writing cross-platform code, but
      // is not required. All shared libraries are set to start with
      // "lib" and end with "extension".
      std::string library_path = "/usr/lib/libxml2.so";

      // Create shared_library object with the relative or absolute
      // path to the shared library.
      shared_library lib(library_path);

      // Attempt to open the shared library.
      if (!lib.open()) {
        std::cerr << "Library failed to open: " << library_path << std::endl;
        return;
      }
      // Retrieve a function from the library, and store it in a Boost.Function
       // object. It is also possible to use function pointers, but the syntax
       // for Boost.Function is easier to understand. This retrieves a function
       // called "boost_extension_hello_world" with a void return type and a single
       // parameter of type int.
//       typedef boost::function<void (int)> FuncType;
//         FuncType f(lib.get<FuncType::result_type,
//                            FuncType::arg1_type>("boost_extension_hello_world"));
         typedef boost::function<void ()> FuncType ;
           FuncType f;
           getMe( "boost_extension_hello_world", &lib, f);
       // Check that the function was found.
       if (!f) {
         std::cerr << "Function not found!" << std::endl;
         return ;
       }

       // Call the function from the shared library with
       // an integer parameter.
       f();

//       LUS::SpringUnitSyncLib s;
//       s.Load( "", "" );
}

