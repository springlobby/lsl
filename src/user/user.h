#ifndef LIBSPRINGLOBBY_HEADERGUARD_USER_H
#define LIBSPRINGLOBBY_HEADERGUARD_USER_H

#include <utils/global_interfaces.h>
#include <string>

namespace LSL {

class User : public HasIndex< std::string >
{
public:
    User() {}

    std::string index() const {return "dummy";}
    static std::string className() { return "Channel"; }
};

} // namespace LSL

#endif // LIBSPRINGLOBBY_HEADERGUARD_USER_H
