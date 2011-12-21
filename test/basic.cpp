#include <container/userlist.h>

int main(int, char**)
{
    using namespace LSL;
    UserList userlist;
    UserList::ItemType* k = new UserList::ItemType();
    UserList::PointerType p( k );
    userlist.Add( p );
//    userlist.Remove
	return 0;
}
