#ifndef CHANNEL_H
#define CHANNEL_H

#include <lslutils/global_interfaces.h>
#include <lslutils/type_forwards.h>

namespace LSL {

class Channel : public HasKey< std::string >
{
public:
    Channel();

    std::string index() const { return "dummy"; }
    static std::string className() { return "Channel"; }

	std::string Name() const { return "dummy"; }

    void OnChannelJoin( const ConstUserPtr user );
};

} // namespace LSL {

#endif // CHANNEL_H
