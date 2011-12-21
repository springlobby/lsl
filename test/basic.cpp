#include <container/userlist.h>

int main(int, char**)
{
    using namespace LSL;
    UserList userlist;
    UserList::PointerType p( new UserList::ItemType() );
    userlist.Add( p );
//    userlist.Remove
	return 0;
}
