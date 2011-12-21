#ifndef LIBSPRINGLOBBY_HEADERGUARD_IBATTLE_H
#define LIBSPRINGLOBBY_HEADERGUARD_IBATTLE_H

#include <utils/global_interfaces.h>

namespace LSL {

class IBattle : public HasIndex< int >
{
public:
    IBattle() {}

    int index() const { return 1; }
};

} // namespace LSL

#endif // LIBSPRINGLOBBY_HEADERGUARD_IBATTLE_H
