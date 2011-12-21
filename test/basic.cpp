#include <container/userlist.h>
#include <container/battlelist.h>
#include <container/channellist.h>

#define TESTLIST(name) \
    { name instance; \
    name :: ItemType* k = new name :: ItemType();  \
    name :: PointerType p( k ); \
    instance.Add( p ); }

int main(int, char**)
{
    using namespace LSL;
    TESTLIST(UserList)
    TESTLIST(BattleList)
    TESTLIST(ChannelList)

	return 0;
}
