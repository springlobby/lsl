#ifndef CHANNEL_H
#define CHANNEL_H

#include <lslutils/global_interfaces.h>
#include <lslutils/type_forwards.h>

namespace LSL {

class Channel : public HasKey< std::string >
{
public:
    Channel();
    Channel(const std::string& name);

    std::string key() const { return Name(); }
    static std::string className() { return "Channel"; }

	std::string Name() const { return "dummy"; }

    void OnChannelJoin( const ConstUserPtr user );

    void SetNumUsers( size_t numusers );
    void SetTopic( const std::string topic);

};

} // namespace LSL {

#endif // CHANNEL_H
