#include <container/userlist.h>
#include <container/battlelist.h>
#include <container/channellist.h>
#include "common.h"
#include "commands.h"
#include <iostream>
#include <utils/misc.h>

#define TESTLIST(name) \
    { name instance; \
    name :: ItemType* k = new name :: ItemType();  \
    name :: PointerType p( k ); \
    instance.Add( p );\
    assert( instance.size() == 1 );\
    instance.Remove( p->index() );\
    assert( instance.size() == 0 );\
    try {\
        instance.Get( p->index() );\
        throw TestFailedException("Get should've thrown MissingItemException");\
    }\
    catch (LSL::name::MissingItemException& e) {std::cerr << e.what() << std::endl;}\
    catch (std::exception& b) { std::cerr << b.what() << std::endl; \
        throw TestFailedException("Get should've thrown MissingItemException"); } \
    }

//#include <unitsync++/c_api.h>
void dummySync();
int main(int, char**)
{
    using namespace LSL;
    TESTLIST(UserList)
    TESTLIST(BattleList)
//    TESTLIST(ChannelList)


	dummySync();

//	std::string sequence("/root/path/jijij.png");
//	std::cout	<< "\n\nBeforeLast: " << LSL::Util::BeforeLast( sequence, "/" ) << "\n"
//				<< "AfterLast: " << LSL::Util::AfterLast( sequence, "/" ) << "\n"
//				<< "BeforeFirst: " << LSL::Util::BeforeFirst( sequence, "/" ) << "\n"
//				<< "AfterFirst: " << LSL::Util::AfterFirst( sequence, "/" ) << "\n";

	std::cout << std::endl;
	return 0;
}
